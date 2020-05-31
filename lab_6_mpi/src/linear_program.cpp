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

void count_next_step(const m_matrix<double> &previous,
                     m_matrix<double> &current,
                     const ConfigFileOpt &config) {
    // count next area for the "current" field based on previous
    auto c = previous.get_cols() - 1, r = previous.get_rows() - 1;
    for (size_t i = 1; i < r; ++i) {
        for (size_t j = 1; j < c; ++j) {
            double x = (previous.get(i - 1, j) - (2 * previous.get(i, j)) + previous.get(i + 1, j)) /
                       std::pow(config.get_delta_x(), 2);
            double y = (previous.get(i, j - 1) - (2 * previous.get(i, j)) + previous.get(i, j + 1)) /
                       std::pow(config.get_delta_y(), 2);
            current.set(i, j, previous.get(i, j) + config.get_delta_t() * config.get_alpha() * (x + y));
        }
    }
#ifdef DEBUG
//    std::cout << "--------prev" << std::endl;
//    previous.print();
//    std::cout << "--------current" << std::endl;
//    current.print();
#endif
}

void linear_program(m_matrix<double> matrix, const ConfigFileOpt &config) {
    m_matrix<double> second_matrix = matrix;
    size_t counter = 0;
    bool flag = false;

    // for gif creation
    GifWriter gif_w;
    size_t delay = 50;
    GifBegin(&gif_w, "heatmap.gif", 100, 100, delay);

    while (!check_thermal_balance(matrix)) {
        if (flag) {
            count_next_step(matrix, second_matrix, config);
            flag ^= true;
        } else {
            count_next_step(second_matrix, matrix, config);
            flag ^= true;
        }
        if (!((counter++) % config.get_data_cycles())) {
//            std::cout << "--------matrix" << std::endl;
            std::cout << counter << std::endl;
//            matrix.print();
            char name[100];
            sprintf(name, "res/%zu.png", counter);
            write_to_png(name, matrix, &gif_w);
        }

    }
    GifEnd(&gif_w);
}
