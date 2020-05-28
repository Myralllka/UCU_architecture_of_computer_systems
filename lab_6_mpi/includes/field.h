//
// Created by myralllka on 5/28/20.
//

#ifndef MPI_HEAT_TRANSFER_FIELD_H
#define MPI_HEAT_TRANSFER_FIELD_H

#include "m_matrix.h"

class field {
    m_matrix<double> data;
    size_t width;
    size_t height;
public:
    field() = delete;

    explicit field(const std::string &filename) {
        size_t cols = 0, rows = 0;
        std::string line;
        double tmp;
        std::ifstream in(filename);
        in >> rows >> cols;
        data = m_matrix<double>(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                in >> tmp;
                data.set(i, j, tmp);
            }
        }
    }

    ~field() = default;

    [[nodiscard]] size_t get_cols() const {
        return width;
    }

    [[nodiscard]] size_t get_rows() const {
        return height;
    }


};


#endif //MPI_HEAT_TRANSFER_FIELD_H
