//
// Created by solia on 5/30/20.
//

#include "visualization.h"
#include <png.h>

std::vector<size_t> to_rgb(size_t min, size_t max, double value) {
    double f_max = static_cast<double>(max);
    double f_min = static_cast<double>(min);
    double ratio = 2 * (value - f_min) / (f_max - f_min);

    std::vector<size_t> rgb;
    size_t b = static_cast<size_t>(std::max(0.0, 255 * (1 - ratio)));
    size_t r = static_cast<size_t>(std::max(0.0, 255 * (ratio - 1)));
    rgb.push_back(r);
    rgb.push_back(255 - b - r); // g
    rgb.push_back(b);

    return rgb;
}

void write_to_png(const std::string &f_name, m_matrix<double> &to_vis) {
    FILE * file_ptr = fopen(f_name.data(), "wb");
    if (!file_ptr) {
        throw VisualizationException("invalid to-write-to file specified");
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr){
        throw VisualizationException("failed to create PNG structure");
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        throw VisualizationException("failed to create info structure");
    }

    int width = to_vis.get_rows();
    int height = to_vis.get_cols();
    int bit_depth = 8; // 8-bit depth
    png_init_io(png_ptr, file_ptr);
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth,
            PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    auto rows_ptr = (png_bytepp) png_malloc(png_ptr, sizeof(png_bytepp) * height);
    for (size_t i = 0; i < height; i++) {
        rows_ptr[i] = (png_bytep) png_malloc(png_ptr, width * 3);
    }

    std::vector<size_t> rgb_value;
    for (size_t i = 0; i < height; i++) {  // row
        for (size_t j = 0; j < width; j++) { //column
            rgb_value = to_rgb(0, 100, to_vis.get(i, j));

            for (size_t k = 0; k < 3; k++)
                rows_ptr[i][j * 3 + k] = rgb_value[k];
        }
    }

    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, rows_ptr);
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
}
