#include <iostream>
#include <thread>
#include <vector>

class Test {
public:
    Test() { std::cout << "Constructed\n"; }
    ~Test() { std::cout << "Destroyed\n"; }
};

int main() {
    std::vector<int> v;
    v.push_back(5);
    v.push_back(2);
    v.push_back(1);
    for (auto &x:v) {
        std::cout << x << std::endl;
    }
//    int *array = new int[100];
//    int *variable = new int;
//    *variable = 10;
//    delete variable;
//    delete[] array;
//    size_t N;
//    std::cin >> N;
//    int *arr = new int[N];
    return 0;
}
