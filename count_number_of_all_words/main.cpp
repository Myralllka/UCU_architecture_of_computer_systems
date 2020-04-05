#include <iostream>
#include <chrono>
#include <map>
#include "includes/config_file.h"
#include "includes/linear_program.h"
#include "includes/parallel_program.h"
#include "includes/parser_exeption.h"
#include <boost/locale.hpp>
#include "includes/speed_tester.h"

int main(int argc, char *argv[]) {
    //  ##################### Program Parameter Parsing ######################
    // TODO: should be config.conf in current directory
    std::string filename = "./config.conf";
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
        throw OptionsParseException();
    }

    //  #####################    Generate global locale       ######################
    boost::locale::generator gen;
    std::locale loc = gen("");
    std::locale::global(loc);

    auto total_time = get_current_time_fenced();
    if (threads > 1) {
        parallel_count(infile, out_by_a_filename, out_by_n_filename, threads);
    } else {
        linear_count(infile, out_by_a_filename, out_by_n_filename);
    }
    auto finish_time = get_current_time_fenced();
//    std::cout << infile << out_by_a_filename << out_by_n_filename << threads << std::endl;
    std::cout << "Total: "<< to_s(finish_time - total_time) << '.'<< to_ms(finish_time - total_time)%1000 << std::endl;
    return 0;
}
