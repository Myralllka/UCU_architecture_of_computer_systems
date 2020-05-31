//
// Created by myralllka on 5/28/20.
//

#include "linear_program.h"
#include "code_controle.h"
#include "visualization.h"


static bool check_thermal_balance(const m_matrix<double> &field, const double &epsilon) {
    auto prev = &field.get(0, 0);
    for (int i = 0; i < static_cast<int>(field.get_rows() * field.get_cols()); ++i) {
        if (std::abs(*prev - *(prev + i)) > epsilon) {
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

void linear_program(m_matrix<double> matrix, const ConfigFileOpt &config) {
    m_matrix<double> second_matrix = matrix;
    size_t counter = 0;
    bool flag = false;

    // for gif-h creation
    GifWriter gif_w{};
    size_t delay = 50;
    GifBegin(&gif_w, "heatmap.gif-h", matrix.get_cols(), matrix.get_rows(), delay);

    while (!check_thermal_balance(matrix)) {
        if (flag)
            count_next_step(matrix, second_matrix, config);
        else
            count_next_step(second_matrix, matrix, config);
        flag ^= true;

        if (!((counter++) % config.get_data_cycles())) {
            char name[100];
            sprintf(name, "res/%zu.png", counter);
            write_to_png(name, matrix, gif_w);
        }
    }
    GifEnd(&gif_w);
}
