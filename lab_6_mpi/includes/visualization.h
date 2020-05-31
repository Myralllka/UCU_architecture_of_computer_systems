//
// Created by solia on 5/30/20.
//

#ifndef ARCHITECTURE_OF_COMPUTER_SYSTEMS_VISUALIZATION_H
#define ARCHITECTURE_OF_COMPUTER_SYSTEMS_VISUALIZATION_H

#include <png.h>
#include <string>
#include <iostream>
#include <vector>

#include "gif-h/gif.h"
#include "m_matrix.h"
#include "exceptions/visualization_exception.h"

std::vector<size_t> to_rgba(size_t min, size_t max, double value);

void assert_valid_rgba(std::vector<size_t> &rgba);

void write_to_png(const std::string &f_name, const m_matrix<double>& to_vis, GifWriter &gif_w);

#endif //ARCHITECTURE_OF_COMPUTER_SYSTEMS_VISUALIZATION_H
