//
// Created by myralllka on 3/25/20.
//
#include "../includes/linear_program.h"
#include "../includes/extractor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <boost/locale.hpp>

//namespace locale=boost::locale;

void count_words(std::string &input_filename, std::string &output_filename_a, std::string &output_filename_n) {
    std::ifstream raw_file(input_filename, std::ios::binary);
    auto buffer = [&raw_file] {
        std::ostringstream ss{};
        ss << raw_file.rdbuf();
        return ss.str();
    }();

    std::cout << output_filename_a << output_filename_n << std::endl;

//    boost::locale::localization_backend_manager lbm
//            = boost::locale::localization_backend_manager::global();
//    auto s = lbm.get_all_backends();
//    for_each(s.begin(), s.end(),
//             [](std::string& x){ std::cout << x << std::endl; });
    extract_to_memory(buffer);
    std::string grussen = "grüßEN";
    std::cout   <<"Upper "<< boost::locale::to_upper(grussen) << std::endl
                <<"Lower "<< boost::locale::to_lower(grussen) << std::endl
                <<"Title "<< boost::locale::to_title(grussen) << std::endl
                <<"Fold  "<< boost::locale::fold_case(grussen) << std::endl;
}
