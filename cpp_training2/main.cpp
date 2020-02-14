#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <map>

using std::cout;
using std::endl;
using std::vector;

class myString {
    char *data_m = nullptr;
    size_t size_m = 0, capacity_m = 0;
public :
    myString(const char *str) {
        data_m = new char[capacity_m];
        size_m = strlen(str);
        capacity_m = size_m + 1;
        strcpy(data_m, str);
        // string s{"sx"};
    }

    myString() : capacity_m(16) {
        data_m = new char[capacity_m];
    }

    ~myString() {
        delete[] data_m;
    }

    size_t size() const { //const told to the compiler that method don`t change the instance
        return size_m;
    };

    char &operator[](size_t i) {
        return data_m[i];
    }

    const char &operator[](size_t i) const {
        return data_m[i];
    }

    // copy
    myString(const myString &arg) :
            capacity_m(arg.capacity_m),
            size_m(arg.capacity_m) {
        data_m = new char[capacity_m];
        for (size_t i = 0; i < size_m; ++i) {
            data_m[i] = arg.data_m[i];
        }
    }

    void swap(myString &other) {
        std::swap(size_m, other.size_m);
        std::swap(capacity_m, other.capacity_m);
        std::swap(data_m, other.data_m);
    }

    myString &operator=(const myString &arg) {
        myString temp{arg};
        temp.swap(*this);
        return *this;
    }

};

int main() {


//    std::map<std::string, int> d;
//    d["abc"] = 5;
//    d["xxx"] = 7;
//    d["yyy"] = 7;
//    for (auto &x:d) {
//        cout << x.first << ": "<< x.second << endl;
//    }
//
//
//    read file to the memory
//
//    bit good (success previous read),
//    bit fail (don`t read/write; can clear this bit and retry),
//    bit bad (damaged)
//    bit EOF (can not read from file anymore)
//    read line std::getline() function
//
//    vector<int> v;
//    std::ifstream file("text.txt");
//    if (file) {
//        cout << "good" << endl;
//    }
//    int x;
//    while (file >> x) {
//        v.push_back(x);
//    }
//    for (auto &y:v) {
//        cout << y << ",";
//    }
//    cout << endl;

//    cout << "Hello, World!" << endl;
//    vector<int> v;
//    for (auto &x:v) {
//        cout << x << endl;
//    }
//    int arr[5] = {3, 6, 11, 4, 5};
//    for (int *p = arr; p != arr + 5; ++p) {
//        cout << *p << endl;
//    }
//    for (vector<int>::iterator p = v.begin(); p != v.end(); ++p) {
//        cout << *p << endl;
//    }
    return 0;
}
