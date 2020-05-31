//
// Created by myralllka on 5/28/20.
//

#ifndef MPI_HEAT_TRANSFER_LINEAR_PROGRAM_H
#define MPI_HEAT_TRANSFER_LINEAR_PROGRAM_H

#include "files/config_file.h"
#include "m_matrix.h"

void linear_program(m_matrix<double> matrix, const ConfigFileOpt &config);

void count_next_step(const m_matrix<double> &previous,
                     m_matrix<double> &current,
                     const ConfigFileOpt &config);

#endif //MPI_HEAT_TRANSFER_LINEAR_PROGRAM_H
