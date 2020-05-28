#include <iostream>
#include <string>
#include <boost/mpi.hpp>
#include <filesystem>

#include "files/config_file.h"
#include "m_matrix.h"
#include "linear_program.h"

ConfigFileOpt parse_args(int argc, char **argv);

void assert_valid_config(const ConfigFileOpt& conf);


int main(int argc, char *argv[]) {
    ConfigFileOpt config = parse_args(argc, argv);
    assert_valid_config(config);
    m_matrix<double> tmp(config.get_field_filename()); // load matrix

    boost::mpi::environment env{argc, argv};
    boost::mpi::communicator world{};

//    if (world.rank() == 0) {
//        int i;
//        world.recv(1, 16, i);
//        std::cout << i << std::endl;
//    } else if (world.rank() == 1) {
//        world.send(0, 16, 99);
//    }
    std::cout << "rank " << world.rank() << " of " << world.size() << std::endl;


    return 0;
}

ConfigFileOpt parse_args(int argc, char **argv) {
    //  ##################### Program Parameter Parsing ######################
    std::string filename = "config.conf";
    if (argc == 2) {
        filename = argv[1];
    } else if (argc > 2) {
        std::cerr << "Too many arguments. Usage: \n"
                     "\tprogram [config-filename]\n" << std::endl;
        exit(1);
    }
    //  #####################    Config File Parsing    ######################
    ConfigFileOpt config;
    try {
        config.parse(filename);
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        exit(3);
    }
    return config;
}

void assert_valid_config(const ConfigFileOpt& conf) {
    // TODO: rewrite through exceptions
    if (!std::filesystem::exists(conf.get_field_filename())) {
        std::cerr << "Error: File or Directory '" << conf.get_field_filename() << "' do not exist (or can not be created)!"
                  << std::endl;
        exit(21);
    } else if (conf.get_field_filename().empty()) {
        std::cerr << "Error: Field file is empty or missing field file filename!" << std::endl;
        exit(23);
    }
}
