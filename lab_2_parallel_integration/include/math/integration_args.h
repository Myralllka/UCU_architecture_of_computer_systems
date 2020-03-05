//
// Created by fenix on 3/1/20.
//

#ifndef LAB_2_PARALLEL_INTEGRATION_INTEGRATION_ARGS_H
#define LAB_2_PARALLEL_INTEGRATION_INTEGRATION_ARGS_H

#include "func_args.h"
#include <cstddef>

struct integration_args {
    const point start;
    const point end;
    const f_params conf;
    const uint16_t flow_n;
//    const size_t steps;
};

#endif //LAB_2_PARALLEL_INTEGRATION_INTEGRATION_ARGS_H
