//
// Created by myralllka on 5/28/20.
//

#include "linear_program.h"
#include "code_controle.h"
#include "visualization.h"
#include "sstream"
#define EPSILON 10

bool check_thermal_balance(m_matrix<double> &field) {
    auto prev = &field.get(0, 0);
    for (int i = 0; i < field.get_rows() * field.get_cols(); ++i) {
        if (std::abs(*prev - *(prev + i)) > EPSILON) {
            return false;
        }
    }
    return true;
}

void count_next_step(const m_matrix<double> &previous, m_matrix<double> &current, const ConfigFileOpt &config) {
    // count next area for the "current" field based on previous
    const auto c = previous.get_cols() - 1, r = previous.get_rows() - 1;
    for (size_t row = 1; row < r; ++row) {
        for (size_t col = 1; col < c; ++col) {
            double x = (previous.get(row - 1, col) - (2 * previous.get(row, col)) + previous.get(row + 1, col)) /
                       std::pow(config.get_delta_x(), 2);
            double y = (previous.get(row, col - 1) - (2 * previous.get(row, col)) + previous.get(row, col + 1)) /
                       std::pow(config.get_delta_y(), 2);
            current.set(row, col, previous.get(row, col) + config.get_delta_t() * config.get_alpha() * (x + y));
        }
    }
#ifdef DEBUG
//    std::cout << "--------prev" << std::endl;
//    previous.print();
//    std::cout << "--------current" << std::endl;
//    current.print();
#endif
}


void count_next_step_for_cell(const m_matrix<double> &previous, m_matrix<double> &current,
                              const ConfigFileOpt &config, const int start, const int end) {
    // count next area for the "current" field based on previous
    const auto c = previous.get_cols() - 1;
    double x, y;
    for (int row = start; row < end; ++row) {
        for (int col = 1; col < c; ++col) {
            x = (previous.get(row - 1, col) - (2 * previous.get(row, col)) + previous.get(row + 1, col)) /
                std::pow(config.get_delta_x(), 2);
            y = (previous.get(row, col - 1) - (2 * previous.get(row, col)) + previous.get(row, col + 1)) /
                std::pow(config.get_delta_y(), 2);
            current.set(row, col, previous.get(row, col) + config.get_delta_t() * config.get_alpha() * (x + y));
        }
    }
}

void linear_program(m_matrix<double> matrix, const ConfigFileOpt &config) {
    m_matrix<double> second_matrix = matrix;
    size_t counter = 0;
    bool flag = false;
    while (!check_thermal_balance(matrix)) {
        if (flag)
            count_next_step(matrix, second_matrix, config);
        else
            count_next_step(second_matrix, matrix, config);
            flag ^= true;
        }
        if (!((counter++) % config.get_data_cycles())) {
//            std::cout << "--------matrix" << std::endl;
            std::cout << counter << std::endl;
//            matrix.print();
            char *name;
            sprintf(name, "res/%zu.png", counter);
            write_to_png(name, matrix);
        }
    }
}
