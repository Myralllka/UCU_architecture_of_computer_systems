//
// Created by fenix on 2/22/20.
//

#ifndef TIC_TAC_TOE_TTT_GAME_H
#define TIC_TAC_TOE_TTT_GAME_H


#include <vector>
#include <cstdint>
#include "gameplay/Player.h"
#include "ui/AbstractUI.h"

class TTT_Game {
    std::vector<Player> players = std::vector<Player>(2);
    uint8_t current_pl = 0u;
    AbstractUI *ui_interf;

    TTT_Game(AbstractUI *new_interface);

    void _init_players(); // TODO: intro + player init

public:
    static TTT_Game create(const char visual_mode); // TODO: create appropriate UI for the given value form UIModes enum

    ~TTT_Game();  // TODO: destructor of AbstractUI allocated object

    void play(); // TODO: method init and start the game
};


#endif //TIC_TAC_TOE_TTT_GAME_H
