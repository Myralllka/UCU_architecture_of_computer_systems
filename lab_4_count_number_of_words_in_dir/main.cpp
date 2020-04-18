#include <iostream>
#include <chrono>
#include <map>
#include "includes/files/config_file.h"
#include "includes/counting/linear_program.h"
#include "includes/counting/parallel_program.h"
#include "includes/exceptions/parser_exeption.h"
#include <boost/locale.hpp>
#include <boost/filesystem.hpp>
#include "includes/speed_tester.h"
#include "includes/files/file_interface.h"
#include "includes/queues/tqueue.h"

#include "includes/debug_control.h"

int main(int argc, char *argv[]) {
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

    if (infile.empty() || out_by_a_filename.empty() or out_by_n_filename.empty() || threads < 1) {
        std::cerr << "Error: Config file is empty or missing some values!" << std::endl;
        exit(23);
    }
    std::ofstream outfile_alpha;
    std::ofstream outfile_number;
    outfile_alpha.open(out_by_a_filename);
    outfile_number.open(out_by_n_filename);
    outfile_alpha.close();
    outfile_number.close();

    if (!boost::filesystem::exists(infile) || !boost::filesystem::exists(out_by_a_filename) ||
        !boost::filesystem::exists(out_by_n_filename)) {
        std::cerr << "Error: File or Directory '" << infile << "' do not exist (or can not be created)!" << std::endl;
        exit(21);
    }


    //  #####################  Generate List Files to Process ######################
    std::vector<std::string> files_list;
    if (boost::filesystem::is_directory(infile)) {
        // WARNING: do not list empty files!!!
        files_list = list_all_files_from(infile);
    } else {
        files_list.push_back(infile);
    }

    //  #####################    Generate global locale       ######################
    boost::locale::generator gen;
    std::locale loc = gen("");
    std::locale::global(loc);

    //  #####################    Load Files from Disk         ######################
    auto start_load = get_current_time_fenced();
    auto end_load = get_current_time_fenced();


    //  #####################    Count words in Text         ######################
    if (threads > 1) {
        t_queue<file_packet> data_queue;
        std::thread file_loader_thread{read_files_to<std::vector<std::string>, t_queue<file_packet>>,
                                       std::ref(files_list), &data_queue};
        parallel_count(data_queue, out_by_a_filename, out_by_n_filename, threads);
        file_loader_thread.join();
    } else {
        std::vector<std::string> data;
        linear_count(files_list, out_by_a_filename, out_by_n_filename);
    }
    auto finish_time = get_current_time_fenced();
//    std::cout << infile << out_by_a_filename << out_by_n_filename << threads << std::endl;
    std::cout << "Total: " << to_us(finish_time - start_load) << std::endl;
#ifdef DEBUG_INFO
    std::cout << "Total: " << to_s(finish_time - start_load) << '.' << to_ms(finish_time - start_load) % 1000
              << std::endl;
#endif
    return 0;
}
