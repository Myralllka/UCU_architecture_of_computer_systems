#include <iostream>
#include <string>

#include "files/config_file.h"
#include "m_matrix.h"
#include "linear_program.h"
#include "field.h"

//ConfigFileOpt parse_args(int argc, char **argv);

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


int main(int argc, char *argv[]) {
    ConfigFileOpt config = parse_args(argc, argv);

    const size_t specific_heat_capacity = config.get_specific_heat_capacity();
    const size_t thermal_conduction = config.get_thermal_conduction();
    const size_t density = config.get_density();
    const size_t width = config.get_width();
    const size_t height = config.get_height();
    const double delta_x = config.get_delta_x();
    const double delta_y = config.get_delta_y();
    const double delta_t = config.get_delta_t();
    const size_t data_cycles = config.get_data_cycles();
    const std::string field_filename = config.get_field_filename();

    ////////////////////////////// ValidateParameters ///////////////////////////////
    auto a = config.get_thermal_conduction() / config.get_density() / config.get_specific_heat_capacity();
    auto coef = std::pow(std::max(config.get_delta_y(), config.get_delta_x()), 2) / 8 / a;
    if (!boost::filesystem::exists(field_filename)) {
        std::cerr << "Error: File or Directory '" << field_filename << "' do not exist (or can not be created)!"
                  << std::endl;
        return 21;
    } else if (field_filename.empty()) {
        std::cerr << "Error: Field file is empty or missing field file filename!" << std::endl;
        return 23;
    } else if (config.get_delta_t() < coef) {
        std::cerr << "Error: parameters does not meet the Von Neumann criterion!" << std::endl;
        return 23;
    }
    /////////////////////////////////////////////////////////////////////////////////
    m_matrix<double> tmp(field_filename);

    linear_program(tmp, config);
    tmp.print();
    return 0;
}