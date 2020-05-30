//
// Created by myralllka on 5/26/20.
//

#ifndef CNN_CONVOLUTION_2D_M_MATRIX_H
#define CNN_CONVOLUTION_2D_M_MATRIX_H

#include <cstring>
#include <algorithm>
#include <fstream>

#include "code_controle.h"

#if defined(ERROR_MSG_ON) || defined(DEBUG)

#include <iostream>

#endif // ERROR_MSG_ON || DEBUG

#ifdef DEBUG

#include <exeptions/index_exception.h>

#endif // DEBUG

template<typename T>
class m_matrix {
    size_t rows{}, cols{};
    T *data;

public:
    m_matrix() = delete;

    m_matrix(size_t row, size_t col) : rows(row), cols(col), data(new T[row * col * sizeof(T)]) {}

    explicit m_matrix(const std::string &input_filename) {
        cols = 0, rows = 0;
        std::string line;
        std::ifstream in(input_filename);
        in >> cols >> rows;
        data = new T[rows * cols * sizeof(T)];
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                in >> data[i * cols + j];
            }
        }
    }

    T *get_process_data_portion(size_t process_index, size_t number_of_processes) {
        // return pointer on the beginning of the data for the process
        // the pointer is directly on the data portion, without any overlaps
        auto main_work = rows / number_of_processes;
        auto extra_work = rows % number_of_processes;
        auto process_block_width = (process_index <= extra_work) ? main_work + 1 : main_work;
        return &(data + (process_block_width * cols));
    }

    size_t get_process_data_portion_length(size_t process_index, size_t number_of_processes) {
        auto main_work = rows / number_of_processes;
        auto extra_work = rows % number_of_processes;
        auto process_block_width = (process_index <= extra_work) ? main_work + 1 : main_work;
        return cols * process_block_width;
    }

#if defined(ERROR_MSG_ON) || defined(DEBUG)

    void print_data() {
        for (size_t n = 0; n < cols * rows; ++n) {
            std::cout << data[n] << " ";
        }
    }

#endif // ERROR_MSG_ON || DEBUG

    ~m_matrix() {
        delete[] data;
    }

    m_matrix(const m_matrix &matrix) : rows(matrix.rows), cols(matrix.cols), data(new T[rows * cols * sizeof(T)]) {
        memcpy(data, matrix.data, rows * cols * sizeof(T));
#if defined(ERROR_MSG_ON) || defined(DEBUG)
        std::cerr << "Warning: Use of copy constructor of m_matrix!" << std::endl;
#endif // ERROR_MSG_ON || DEBUG
    }

    m_matrix(m_matrix &&matrix) noexcept: rows(matrix.rows), cols(matrix.cols), data(matrix.data) {
        matrix.data = nullptr;
    }

    m_matrix &operator=(const m_matrix &matrix) {
        if (&matrix != this) {
            if (rows * cols != matrix.rows * matrix.cols) {
                delete[] data;
                data = new T[rows * cols * sizeof(T)];
            }
            rows = matrix.rows;
            cols = matrix.cols;
            data = matrix.data;
            memcpy(data, matrix.data, rows * cols * sizeof(T));
        }
        return *this;
    }

    m_matrix &operator=(m_matrix &&matrix) noexcept {
        rows = matrix.rows;
        cols = matrix.cols;
        data = matrix.data;
        matrix.data = nullptr;
        return *this;
    }

    [[nodiscard]] size_t get_cols() const {
        return cols;
    }

    [[nodiscard]] size_t get_rows() const {
        return rows;
    }

    [[nodiscard]] size_t size() const {
        return rows * cols;
    }

    [[nodiscard]] const T &get(size_t m, size_t n) const {
#ifdef DEBUG
        check_indexes(m, n);
#endif // DEBUG
        return data[m * cols + n];
    }

    void set(size_t m, size_t n, const T &element) {
#ifdef DEBUG
        check_indexes(m, n);
#endif // DEBUG
        data[m * cols + n] = element;
    }

    void set(size_t m, size_t n, T &&element) {
#ifdef DEBUG
        check_indexes(m, n);
#endif // DEBUG
        data[m * cols + n] = std::move(element);
    }

#ifdef DEBUG

    void print() const {
        for (size_t m = 0; m < this->rows; ++m) {
            for (size_t n = 0; n < this->cols; ++n) {
                std::cout << data[m * cols + n] << " ";
            }
            std::cout << std::endl;
        }
    }

#endif // DEBUG

private:
#ifdef DEBUG

    void check_indexes(size_t m, size_t n) const {
        if (m > rows or n > cols) {
#ifdef ERROR_MSG_ON
            std::cerr << "Array out of bounds access!" << std::endl;
            std::cerr << (m > rows ? "incorrect number of rows" : "incorrect number of columns") << std::endl;
#endif // ERROR_MSG_ON
            throw IndexException{};
        }
    }

#endif // DEBUG
};

#endif //CNN_CONVOLUTION_2D_M_MATRIX_H
