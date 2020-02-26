//
// Created by fenix on 2/22/20.
//

#ifndef TIC_TAC_TOE_CONSOLEUI_H
#define TIC_TAC_TOE_CONSOLEUI_H


#include "AbstractUI.h"

class ConsoleUI : public AbstractUI {
public:
    ConsoleUI(const Field &new_field);

    void render_intro() const override;

    std::string get_pl_name(uint8_t pl_number) const override;

    void render_field() const override;

    std::vector<uint8_t> get_move() const override;

    void render_result() const override;

    void close() const override;

};


#endif //TIC_TAC_TOE_CONSOLEUI_H
