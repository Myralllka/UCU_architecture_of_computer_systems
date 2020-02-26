//
// Created by myralllka on 2/21/20.
//

#ifndef TIC_TAC_TOE_PLAYER_H
#define TIC_TAC_TOE_PLAYER_H


#include <string>

class Player {
public:
    const std::string name;
    const char character; // TODO: should be assigned from PlayerCharacters
    Player(std::string& n_name, const char n_char);
    ~Player();
};


#endif //TIC_TAC_TOE_PLAYER_H
