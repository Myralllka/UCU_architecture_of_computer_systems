//
// Created by myralllka on 3/25/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_CONFIG_FILE_H
#define COUNT_NUMBER_OF_ALL_WORDS_CONFIG_FILE_H

#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

#include "exeptions/parser_exeption.h"

class ConfigFileOpt {
public:
    ConfigFileOpt();

    ~ConfigFileOpt() = default;

    void parse(const std::string &file_name);

    // make getters
    const size_t& get_specific_heat_capacity() const { return specific_heat_capacity; }

    const size_t& get_thermal_conduction() const { return thermal_conduction; }

    const size_t& get_density() const { return density; }

    const size_t& get_height() const { return height; }

    const size_t& get_width() const { return width; }

    const double& get_delta_x() const { return delta_x; }

    const double& get_delta_y() const { return delta_y; }

    const double& get_delta_t() const { return delta_t; }

    const size_t& get_data_cycles() const { return data_cycles; };

    const std::string &get_field_filename() const { return field_filename; };
private:
    void init_opt_description();

    static const std::string& assert_file_exist(const std::string &f_name);

    // declare all parameters
    size_t specific_heat_capacity = 0;
    size_t thermal_conduction = 0;
    size_t density = 0;
    size_t height = 1000;
    size_t width = 1000;
    double delta_x = 0;
    double delta_y = 0;
    double delta_t = 0;
    size_t data_cycles = 0;
    std::string field_filename;

    boost::program_options::options_description opt_conf{"Config File Options"};
    boost::program_options::variables_map var_map{};
};

#endif //COUNT_NUMBER_OF_ALL_WORDS_CONFIG_FILE_H
