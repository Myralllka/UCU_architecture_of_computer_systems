//
// Created by solia on 5/30/20.
//

#include "visualization.h"
#include "code_controle.h"

std::vector<size_t> to_rgba(size_t min, size_t max, double value) {
    auto f_max = static_cast<double>(max);
    auto f_min = static_cast<double>(min);

    if (value > f_max or value < f_min)
        throw VisualizationException("temperature value out of visualization range");

    double ratio = 2 * (value - f_min) / (f_max - f_min);

    std::vector<size_t> rgba;
    size_t b = static_cast<size_t>(std::max(0.0, 255 * (1 - ratio)));
    size_t r = static_cast<size_t>(std::max(0.0, 255 * (ratio - 1)));
    rgba.push_back(r);
    rgba.push_back(255 - b - r); // g
    rgba.push_back(b);
    rgba.push_back(255); // a

    assert_valid_rgba(rgba);

    return rgba;
}

void assert_valid_rgba(std::vector<size_t> &rgba) {
    for (size_t i = 0; i < 3; i++)
        if (rgba[i] > 256)
            // if invalid range was given - invalid RGB value will be calculated
            throw VisualizationException("invalid RGBA generated");
}

void write_to_png(const std::string &f_name, const m_matrix<double> to_vis, GifWriter &gif_w) {
#ifdef IMG
    FILE * file_ptr = fopen(f_name.data(), "wb");
    if (!file_ptr) {
        throw VisualizationException("invalid to-write-to file specified");
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr){
        // TODO: error: V773 The exception was thrown without closing the file referenced by the 'file_ptr' handle. A resource leak is possible.
        throw VisualizationException("failed to create PNG structure");
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)nullptr);
        throw VisualizationException("failed to create info structure");
    }
#endif
    int width = static_cast<int>(to_vis.get_cols());
    int height = static_cast<int>(to_vis.get_rows());
#ifdef IMG
    png_init_io(png_ptr, file_ptr);
    int bit_depth = 8; // 8-bit depth
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth,
                 PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    auto rows_ptr = (png_bytepp) png_malloc(png_ptr, sizeof(png_bytepp) * height);
    for (size_t i = 0; i < height; i++) {
        rows_ptr[i] = (png_bytep) png_malloc(png_ptr, width * 4);
    }
#endif
    double max_temp = 0;
    for (size_t i = 0; i < to_vis.get_rows(); i++)
        for (size_t j = 0; j < to_vis.get_cols(); j++)
            if (to_vis.get(i, j) > max_temp)
                max_temp = to_vis.get(i, j);

    std::vector<size_t> rgba_value;
    std::vector<uint8_t> pix;
    for (size_t i = 0; i < height; i++) {  // row
        for (size_t j = 0; j < width; j++) { //column
            rgba_value = to_rgba(0, static_cast<size_t>(max_temp), to_vis.get(i, j));

            for (size_t k = 0; k < 4; k++) {
#ifdef IMG
                rows_ptr[i][j * 4 + k] = rgba_value[k];
#endif
                pix.push_back(static_cast<uint8_t>(rgba_value[k]));
            }
        }
    }

    GifWriteFrame(&gif_w, pix.data(), to_vis.get_cols(), to_vis.get_rows(), 50);
#ifdef IMG
    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, rows_ptr);
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
#endif
}
