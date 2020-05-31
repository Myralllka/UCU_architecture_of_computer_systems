//
// Created by solia on 5/30/20.
//

#ifndef ARCHITECTURE_OF_COMPUTER_SYSTEMS_VISUALIZATION_H
#define ARCHITECTURE_OF_COMPUTER_SYSTEMS_VISUALIZATION_H

#include <png.h>
#include <string>
#include <iostream>

#include "m_matrix.h"
#include "exceptions/visualization_exception.h"

std::vector<size_t> to_rgb(size_t min, size_t max, double value);

void write_to_png(const std::string &f_name, m_matrix<double> &to_vis);

#endif //ARCHITECTURE_OF_COMPUTER_SYSTEMS_VISUALIZATION_H
