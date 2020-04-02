#include <iostream>
#include <chrono>
#include <map>
#include "includes/config_file.h"
#include "includes/linear_program.h"
#include <boost/locale.hpp>


int main(int argc, char *argv[]) {
    //  ##################### Program Parameter Parsing ######################
    // TODO: should be config.conf in current directory
    std::string filename = "../config.conf";
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
//    size_t threads = config.get_number_of_threads();

    //  #####################    Generate global locale       ######################
    boost::locale::generator gen;
    std::locale loc = gen("");
    std::locale::global(loc);

    //  #####################    Count words in one thread    ######################
    count_words(infile, out_by_a_filename, out_by_n_filename);
//    std::cout << infile << out_by_a_filename << out_by_n_filename << threads << std::endl;

    return 0;
}
