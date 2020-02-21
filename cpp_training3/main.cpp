#include <iostream>
#include <algorithm>

template<typename T>
class my_vector {
    T *data = nullptr;
    size_t size_m, capacity_m;
public:
    my_vector(size_t in_size) : size_m(in_size),
                                capacity_m(std::max<size_t>(16, in_size)) {
        data = new T[capacity_m];
        for (size_t i = 0; i < size_m; ++i)
            data[i] = T{};
    }

    ~my_vector() {
        delete[] data;
    }

    my_vector(const my_vector &) = delete;

    my_vector &operator=(const my_vector &) = delete;
};

template <typename T, size_t N>
class my_array{
    T data[N];
public:
    constexpr size_t size() const {return N;}
};

int main() {
    my_vector<int> t1{5};
    my_vector<std::string> t2{7};
    return 0;
}
