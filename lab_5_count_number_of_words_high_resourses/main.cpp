#include <iostream>
#include <chrono>
#include <boost/locale.hpp>

#include <boost/filesystem.hpp>
#include <thread>
#include "includes/files/file_interface.h"
#include "includes/files/config_file.h"
#include "includes/speed_tester.h"
#include "includes/counting/linear_program.h"
#include "includes/counting/parallel_program.h"
//#include "includes/code_control.h"
#include "tbb/concurrent_queue.h"

int main(int argc, char *argv[]) {
    auto start_time = get_current_time_fenced();

    //  ##################### Program Parameter Parsing ######################
    std::string filename = "config.dat";
    if (argc == 2) {
        filename = argv[1];
    } else if (argc > 2) {
        std::cerr << "Too many arguments. Usage: \n"
                     "\tprogram [config-filename]\n" << std::endl;
        return 1;
    }

    //  #####################    Config File Parsing    ######################
    ConfigFileOpt config{};
    try {
        config.parse(filename);
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 3;
    }

    const std::string infile = config.get_infile();
    const std::string out_by_a_filename = config.get_out_by_a();
    const std::string out_by_n_filename = config.get_out_by_n();
    const uint8_t threads = config.get_number_of_threads();

    if (infile.empty()) {
        std::cerr << "Error: Config file is empty or missing input filename!" << std::endl;
        return 23;
    }
    if (out_by_a_filename.empty()) {
        std::cerr << "Error: Config file is empty or missing output by alpha filename!" << std::endl;
        return 23;
    }
    if (out_by_n_filename.empty()) {
        std::cerr << "Error: Config file is empty or missing output by number filename!" << std::endl;
        return 23;
    }
    if (threads < 1) {
        std::cerr << "Error: Config file is empty, missing or incorrect threadsnumber!" << std::endl;
        return 23;
    }
    std::ofstream outfile_alpha;
    std::ofstream outfile_number;
    outfile_alpha.open(out_by_a_filename);
    outfile_number.open(out_by_n_filename);
    outfile_alpha.close();
    outfile_number.close();

    for (auto &name:std::vector<std::string>{infile, out_by_a_filename, out_by_n_filename}) {
        if (!boost::filesystem::exists(name)) {
            std::cerr << "Error: File or Directory '" << name << "' do not exist (or can not be created)!" << std::endl;
            return 21;
        }
    }

    //  #####################  Generate List Files to Process ######################
    std::vector<std::string> files_list;
    if (boost::filesystem::is_directory(infile)) {
        // WARNING: do not list empty files!!!
        list_all_files_from(infile, &files_list);
    } else {
        if (!boost::filesystem::is_regular_file(infile)) {
            std::cerr << "Invalid file given " << infile << "!" << std::endl;
            return 22;
        } else if (boost::filesystem::is_empty(infile)) {
            std::cerr << "Empty file given. Nothing to count!" << std::endl;
        } else {
            files_list.push_back(infile);
        }
    }

    //  #####################    Generate global locale       ######################
    boost::locale::generator gen;
    std::locale loc = gen("");
    std::locale::global(loc);

    //  ##############  Load, Unarchive and Count words in Text ####################
    if (threads > 1) {
        tbb::concurrent_queue<file_packet, tbb::cache_aligned_allocator<file_packet>> packet_queue;
//        t_queue <file_packet> packet_queue{static_cast<size_t>(threads) * MAX_LOAD_QUEUE_SIZE};
//        std::thread file_loader_thread{read_files_thread < std::vector < std::string > , t_queue < file_packet >> ,
//                                       std::ref(files_list), &packet_queue};
        std::thread file_loader_thread{
                read_files_thread<std::vector<std::string>, tbb::concurrent_queue<file_packet, tbb::cache_aligned_allocator<file_packet>>>,
                std::ref(files_list), &packet_queue};
        parallel_count(&packet_queue, out_by_a_filename, out_by_n_filename, threads);
        file_loader_thread.join();
    } else {
        linear_count(files_list, out_by_a_filename, out_by_n_filename);
    }
    const auto finish_time = get_current_time_fenced();
    std::cout << "Total: " << to_us(finish_time - start_time) << std::endl;
    return 0;
}
