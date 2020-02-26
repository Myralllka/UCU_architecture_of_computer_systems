//
// Created by fenix on 2/22/20.
//

#include "../../include/ui/ConsoleUI.h"

ConsoleUI::ConsoleUI(const Field &new_field) : AbstractUI(new_field) {

}

void ConsoleUI::render_intro() const {

}

std::string ConsoleUI::get_pl_name(uint8_t pl_number) const {
    return std::string();
}

void ConsoleUI::render_field() const {

}

std::vector<uint8_t> ConsoleUI::get_move() const {
    return std::vector<uint8_t>();
}

void ConsoleUI::render_result() const {

}

void ConsoleUI::close() const {

}
