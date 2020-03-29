//
// Created by myralllka on 3/25/20.
//
#include "../includes/linear_program.h"
#include "../includes/linear_extractor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <boost/locale.hpp>

//namespace locale=boost::locale;

void count_words(std::string &input_filename, std::string &output_filename_a, std::string &output_filename_n) {
    // read entire binary archive into the buffer
    std::ifstream raw_file(input_filename, std::ios::binary);
    std::vector<std::string> data;
    auto buffer = [&raw_file] {
        std::ostringstream ss{};
        ss << raw_file.rdbuf();
        return ss.str();
    }();

    data = extract_to_memory(buffer);


    // ##########################################################
    // IN PROCESS (DIFFERENT TESTING)
    // ##########################################################
    // code from SO that makes all work
//    boost::locale::generator gen;
//    std::locale loc = gen("");
//    std::locale::global(loc);
    // ##########################################################
    //check all existing lbm`s
    boost::locale::localization_backend_manager lbm
            = boost::locale::localization_backend_manager::global();
    auto s = lbm.get_all_backends();
    for_each(s.begin(), s.end(),
             [](std::string& x){ std::cout << x << std::endl; });
    // ##########################################################
    // needed approach (don`t work.)

    lbm.select("icu");

    std::string grussen = "grüßEN";
    std::cout   <<"Upper "<< boost::locale::to_upper(grussen) << std::endl;
//                <<"Lower "<< boost::locale::to_lower(grussen) << std::endl
//                <<"Title "<< boost::locale::to_title(grussen) << std::endl
//                <<"Fold  "<< boost::locale::fold_case(grussen) << std::endl;
}
