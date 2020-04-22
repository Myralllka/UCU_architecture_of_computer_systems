//
// Created by fenix on 4/16/20.
//
#include <boost/filesystem.hpp>
#include <iostream>
#include "includes/archivation/archive_t.h"
#include <string>
#include <vector>


int main() {
    std::cout << "########## TEST ##########" << std::endl;
//    std::cout << boost::filesystem::current_path().string() << ":" << std::endl;
//    int count = 0;
//    for (const auto& file : boost::filesystem::recursive_directory_iterator(boost::filesystem::current_path())) {
//        if (boost::filesystem::is_regular_file(file)) {
//            std::cout << file.path().string() << std::endl;
//            count++;
//        }
//    }
//    std::cout << "Total file num: " << count << std::endl;


//    std::string input_filename = "file.tar";
//    static const std::vector<std::string> supported_formats = {"tar", "gz", "pax", "cpio", "zip", "mtree", "shar", "ar",
//                                                               "raw", "xar", "lha", "lzh", "rar", "7z"};
//    std::string extension = input_filename.substr(input_filename.find_last_of('.') + 1);
//    if (extension == "txt") {
//        std::cout << "OK txt" << std::endl;
//    } else if ([extension]() {
//        for (const auto &el : supported_formats)
//            if (extension == el)
//                return true;
//        return false;
//    }()) {
//        std::cout << "OK archive " << extension << std::endl;
//    } else {
//        std::cerr << "Warning: File '" << input_filename << "' passed as it has a not supported format!" << std::endl;
//    }

//    std::map<std::string, int> map{};
//    map["1"] = 11;
//    map["2"] += 3;
//    std::cout << "map[\"1\"] = " << map["1"] << std::endl;
//    std::cout << "map[\"2\"] = " << map["2"] << std::endl;

    archive_t a{};
    a.load_file("archives/dir.zip");
    std::vector<std::string> vector{};
    a.extract_all(&vector);
    std::cout << "Final vector size: " << vector.size() << std::endl;
    return 0;
}
