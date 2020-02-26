//
// Created by fenix on 2/22/20.
//

#ifndef TIC_TAC_TOE_ABSTRACTUI_H
#define TIC_TAC_TOE_ABSTRACTUI_H


#include <cstdint>
#include <string>
#include <vector>
#include "../gameplay/Field.h"

class AbstractUI {
protected:
    const Field &field;

public:
    AbstractUI(const Field &new_field);

    virtual void render_intro() const = 0;

    virtual std::string get_pl_name(uint8_t pl_number) const = 0;

    virtual void render_field() const = 0;

    virtual std::vector<uint8_t> get_move() const = 0;

    virtual void render_result() const = 0;

    virtual void close() const = 0;

};


#endif //TIC_TAC_TOE_ABSTRACTUI_H
