//
// Created by myralllka on 2/21/20.
//

#include "../../include/gameplay/Player.h"

#include <utility>

Player::Player(std::string& n_name, const char n_char) : name(std::move(n_name)), character(n_char) {

}
Player::~Player() = default;
