//
// Created by myralllka on 5/28/20.
//

#include "linear_program.h"

void
count_next_step(const m_matrix<double> &previous,
                m_matrix<double> &current,
                const double &alpha,
                const ConfigFileOpt &config) {
    // count next area for the "current" field based on previous
    auto c = previous.get_cols() - 1, r = previous.get_rows() - 1;
    for (size_t i = 1; i < r; ++i) {
        for (size_t j = 1; j < c; ++j) {
            double x = (previous.get(i - 1, j) - (2 * previous.get(i, j)) + previous.get(i + 1, j)) /
                       std::pow(config.get_delta_x(), 2);
            double y = (previous.get(i, j - 1) - (2 * previous.get(i, j)) + previous.get(i, j + 1)) /
                       std::pow(config.get_delta_y(), 2);
            current.set(i, j, previous.get(i, j) + config.get_delta_t() * alpha * (x + y));
        }
    }
#ifdef DEBUG
    std::cout << "--------prev" << std::endl;
    previous.print();
    std::cout << "--------current" << std::endl;
    current.print();
#endif
}

void linear_program(m_matrix<double> matrix, const ConfigFileOpt &config) {
    m_matrix<double> second_matrix = matrix;
    for (int i = 0; i < config.get_data_cycles(); ++i) {
        if (i % 2) {
            count_next_step(matrix, second_matrix, config.get_alpha(), config);
        } else {
            count_next_step(second_matrix, matrix, config.get_alpha(), config);
        }
    }

}
