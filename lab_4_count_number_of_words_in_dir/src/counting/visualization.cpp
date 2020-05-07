//
// Created by fenix on 5/3/20.
//

#include "../../includes/counting/visualization.h"
#include <iostream>

void printProgress(double percentage) {
    printf("\r%3d%% [%.*s%*s]", static_cast<int>(percentage * 100), static_cast<int>(percentage * PB_WIDTH), PB_STR,
           PB_WIDTH - static_cast<int>(percentage * PB_WIDTH), "");
    fflush(stdout);
}

void printProgressContinue(double percentage) {
    printf("  %3d%% [%.*s%*s]", static_cast<int>(percentage * 100), static_cast<int>(percentage * PB_WIDTH), PB_STR,
           PB_WIDTH - static_cast<int>(percentage * PB_WIDTH), "");
    std::cout << percentage << std::flush;
    fflush(stdout);
}