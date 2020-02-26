//
// Created by myralllka on 2/21/20.
//

#ifndef TIC_TAC_TOE_CELL_H
#define TIC_TAC_TOE_CELL_H


#include "Player.h"

class Cell {
    const Player *owner = nullptr;
public:
    bool is_empty() const;

    void set_owner(const Player *pl); // TODO: should throw exception IllegalCellAccess
};


#endif //TIC_TAC_TOE_CELL_H
