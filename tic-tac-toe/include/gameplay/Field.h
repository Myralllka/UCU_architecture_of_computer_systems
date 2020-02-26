//
// Created by myralllka on 2/21/20.
//

#ifndef TIC_TAC_TOE_FIELD_H
#define TIC_TAC_TOE_FIELD_H


#include <vector>
#include <cstdint>
#include "Cell.h"
#include "Player.h"

class Field {
    std::vector<std::vector<Cell>> field = std::vector<std::vector<Cell>>{std::vector<Cell>(3),
                                                                          std::vector<Cell>(3),
                                                                          std::vector<Cell>(3)};

    static bool _is_valid_move(uint8_t row, uint8_t col);

public:
    const Player *get_winner() const;

    bool move(uint8_t row, uint8_t col);

};


#endif //TIC_TAC_TOE_FIELD_H
