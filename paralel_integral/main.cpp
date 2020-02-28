#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <map>

inline std::chrono::high_resolution_clock::time_point get_current_time_fenced() {
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D &d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

int main(int argc, char *argv[]) {
    std::map<std::string, double> config;
    std::ifstream cFile("main.conf");
    if (cFile.is_open()) {
        std::string line;
        while (getline(cFile, line)) {
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
            if (line[0] == '#' || line.empty() || line[0] == '[')
                continue;
            auto delimiterPos = line.find('=');
            std::string name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            config[name] = std::stod(value);
        }
    } else {
        std::cerr << "Couldn't open config file for reading.\n";
    }
    for (auto& v:config){
        std::cout << v.first << v.second << std::endl;
    }

//    std::thread th{fn};
//    std::cout << "Hello" << std::endl;
//    th.join();
    return 0;
}
