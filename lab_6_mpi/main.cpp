#include "main.h"

int main(int argc, char *argv[]) {

    //  ##################### Program Parameter Parsing ######################
    std::string filename = "config.conf";
    if (argc == 2) {
        filename = argv[1];
    } else if (argc > 2) {
        std::cerr << "Too many arguments. Usage: \n"
                     "\tprogram [config-filename]\n" << std::endl;
        return 1;
    }

    //  #####################    Config File Parsing    ######################
    ConfigFileOpt config;
    try {
        config.parse(filename);
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 3;
    }

    size_t specific_heat_capacity = config.get_specific_heat_capacity();
    size_t thermal_conduction = config.get_thermal_conduction();
    size_t density = config.get_density();
    size_t height = config.get_height();
    size_t width = config.get_width();
    double delta_x = config.get_delta_x();
    double delta_y = config.get_delta_y();
    double delta_t = config.get_delta_t();
    size_t data_cycles = config.get_data_cycles();
    std::string field_filename = config.get_field_filename();


    for (auto &name:std::vector<std::string>{field_filename}) {
        if (!boost::filesystem::exists(name)) {
            std::cerr << "Error: File or Directory '" << name << "' do not exist (or can not be created)!" << std::endl;
            return 21;
        }
    }

    if (field_filename.empty()) {
        std::cerr << "Error: Field file is empty or missing field file filename!" << std::endl;
        return 23;
    }
    if (width < 1000 or height < 1000) {
        std::cerr << "Error: min field size have to be 1000*1000!" << std::endl;
        return 23;
    }

    return 0;
}
