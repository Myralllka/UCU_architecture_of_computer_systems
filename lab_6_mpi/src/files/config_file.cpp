//
// Created by myralllka on 3/25/20.
//

#include "files/config_file.h"

namespace po = boost::program_options;

ConfigFileOpt::ConfigFileOpt() {
    init_opt_description();
}

void ConfigFileOpt::init_opt_description() {
    opt_conf.add_options()
            ("specific_heat_capacity", po::value<double>(&specific_heat_capacity), "specific heat capacity ")
            ("thermal_conduction", po::value<double>(&thermal_conduction), "thermal conduction")
            ("density", po::value<double>(&density), "density")
            ("height", po::value<size_t>(&height), "field height")
            ("width", po::value<size_t>(&height), "field width")
            ("delta_x", po::value<double>(&delta_x), "delta_x")
            ("delta_y", po::value<double>(&delta_y), "delta_y")
            ("delta_t", po::value<double>(&delta_t), "delta_t")
            ("data_cycles", po::value<size_t>(&data_cycles), "data_cycles")
            ("field_filename", po::value<std::string>(&field_filename), "field filename");
}

void ConfigFileOpt::parse(const std::string &file_name) {
    try {
        std::ifstream conf_file(assert_file_exist(file_name));
        po::store(po::parse_config_file(conf_file, opt_conf), var_map);
        po::notify(var_map);
    } catch (std::exception &E) {
        std::cerr << E.what() << std::endl;
        throw OptionsParseException();
    }
    alpha = get_thermal_conduction() / get_density() / get_specific_heat_capacity();
}

std::string ConfigFileOpt::assert_file_exist(const std::string &f_name) {
    if (!boost::filesystem::exists(f_name)) {
        throw std::invalid_argument("File " + f_name + " not found!");
    }
    return f_name;
}
