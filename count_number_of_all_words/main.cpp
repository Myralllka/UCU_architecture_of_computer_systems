#include <iostream>
#include <chrono>
#include <map>
#include "includes/files/config_file.h"
#include "includes/counting/linear_program.h"
#include "includes/counting/parallel_program.h"
#include "includes/exceptions/parser_exeption.h"
#include <boost/locale.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include "includes/speed_tester.h"
#include "includes/files/read_file.h"

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
//    outfile_alpha << std::endl;
//    outfile_number <<std::endl;
//    for (auto &pair:multimap_of_words) {
//        outfile_number << pair.first << ": " << pair.second << std::endl;
//    }
//    std::vector<std::string> tmp;
//    boost::split(tmp, out_by_a_filename, boost::is_any_of("/"));
//    if ( tmp.size() > 2 && !boost::filesystem::exists(tmp[1])){
//        std::cerr << "Error: File '" << out_by_a_filename << "' do not exist and can not be created!" << std::endl;
//        exit(21);
//    }
//    boost::split(tmp, out_by_n_filename, boost::is_any_of("/"));
//    if ( tmp.size() > 2 && !boost::filesystem::exists(tmp[1])){
//        std::cerr << "Error: File '" << out_by_n_filename << "' do not exist and can not be created!" << std::endl;
//        exit(21);
//    }
    if (!boost::filesystem::exists(infile) || !boost::filesystem::exists(out_by_a_filename) ||
        !boost::filesystem::exists(out_by_n_filename)) {
        std::cerr << "Error: File '" << infile << "' do not exist (or can not be created)!" << std::endl;
        exit(21);
    }

    //  #####################    Generate global locale       ######################
    boost::locale::generator gen;
    std::locale loc = gen("");
    std::locale::global(loc);

    auto total_time = get_current_time_fenced();
    std::vector<std::string> data;
    read_input_file(infile, data);
    if (threads > 1) {
        parallel_count(data, out_by_a_filename, out_by_n_filename, threads);
    } else {
        linear_count(data, out_by_a_filename, out_by_n_filename);
    }
    auto finish_time = get_current_time_fenced();
//    std::cout << infile << out_by_a_filename << out_by_n_filename << threads << std::endl;
    std::cout << "Total: " << to_us(finish_time - total_time) << std::endl;
//    std::cout << "Total: "<< to_s(finish_time - total_time) << '.'<< to_ms(finish_time - total_time)%1000 << std::endl;
    return 0;
}
