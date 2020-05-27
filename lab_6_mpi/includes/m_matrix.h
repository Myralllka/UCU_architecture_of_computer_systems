//
// Created by myralllka on 5/26/20.
//

#ifndef CNN_CONVOLUTION_2D_M_MATRIX_H
#define CNN_CONVOLUTION_2D_M_MATRIX_H

#include <iostream>

#define ALIGNMENT 32

class m_matrix {
    size_t rows;
    size_t cols;
    float *data;
    bool transposed = false;

    void check_indexes(size_t m, size_t n) const {
        if (m > rows or n > cols) {
            std::cerr << "Array out of bounds access!" << std::endl;
            std::cerr << (m > rows ? "incorrect number of rows" : "incorrect number of columns");
            exit(1);
        }
    }

public:

    m_matrix(size_t m, size_t n) : rows(m), cols(n) {
        auto size = n * m;
//        data[size] = float{0};
        data = static_cast<float *>(aligned_alloc(ALIGNMENT, size * sizeof(float)));
        for (int i = 0; i < size; ++i) {
            data[i] = 0;
        }
    }

    ~m_matrix() {
        delete[] data;
    }

    m_matrix(const m_matrix &matrix) = delete;

    m_matrix(m_matrix &&matrix) noexcept: rows(matrix.rows), cols(matrix.cols), data(matrix.data) {
        matrix.data = nullptr;
    }

    m_matrix &operator=(m_matrix &matrix) = delete;

    m_matrix &operator=(m_matrix &&matrix) noexcept {
        if (&matrix != this) {
            rows = matrix.rows;
            cols = matrix.cols;
            data = matrix.data;
            matrix.data = nullptr;
        }
        return *this;
    }

    [[nodiscard]] size_t get_cols() const {
        return transposed ? cols : rows;
    }

    [[nodiscard]] size_t get_rows() const {
        return transposed ? rows : cols;
    }

    [[nodiscard]] const float &get(size_t m, size_t n) const {
        if (transposed) {
            check_indexes(n, m);
            return data[n * cols + m];
        }
        check_indexes(m, n);
        return data[m * cols + n];
    }

    void put(size_t m, size_t n, float element) {
        if (transposed) {
            check_indexes(n, m);
            data[n * cols + m] = element;
        } else {
            check_indexes(m, n);
            data[m * cols + n] = element;
        }
    }

    void transpose() {
        transposed = !transposed;
    }

    m_matrix real_transpose() const {
        m_matrix result(this->cols, this->rows);
        for (size_t m = 0; m < this->rows; ++m) {
            for (size_t n = 0; n < this->cols; ++n) {
                result.data[n * rows + m] = this->data[m * cols + n];
            }
        }
        return result;
    }

    void print() const {
        if (transposed) {
            for (size_t n = 0; n < this->cols; ++n) {
                for (size_t m = 0; m < this->rows; ++m) {
                    std::cout << data[m * cols + n] << " ";
                }
                std::cout << std::endl;
            }
        } else {
            for (size_t m = 0; m < this->rows; ++m) {
                for (size_t n = 0; n < this->cols; ++n) {
                    std::cout << data[m * cols + n] << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    [[nodiscard]] float *get_data(size_t i) const {
        return data + i;
    }

    void print_data() {
        for (size_t i = 0; i < rows * cols; i++) {
            std::cout << data[i] << "|";
        }
        std::cout << std::endl;
    }

    static m_matrix im2col(const std::vector<m_matrix> &src, size_t kernel_size) {
        size_t p = std::pow(kernel_size, 2);
        size_t c = src.size() * p, r = std::pow((src[0].get_rows() - kernel_size + 1), 2);
        size_t s_size = src[0].get_rows() - kernel_size + 1;
        size_t counter_x = 0, counter_y = 0;
        m_matrix result(r, (c / 8 + 1) * 8);
        for (int channel = 0; channel < src.size(); ++channel) {
            for (size_t i = 0; i < s_size; ++i) {
                for (size_t j = 0; j < s_size; ++j) {
                    for (size_t m = 0; m < kernel_size; ++m) {
                        for (size_t n = 0; n < kernel_size; ++n) {
                            result.data[counter_y * (c / 8 + 1) * 8 + (channel * p + counter_x++)] = src[channel].get((m+j), (n+i));
                        }
                    }
                    ++counter_y;
                    counter_x = 0;
                }
            }
            counter_y = 0;
        }
        return result;
    }

};

#endif //CNN_CONVOLUTION_2D_M_MATRIX_H
