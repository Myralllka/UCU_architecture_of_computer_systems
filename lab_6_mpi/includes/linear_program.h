//
// Created by myralllka on 5/28/20.
//

#ifndef MPI_HEAT_TRANSFER_LINEAR_PROGRAM_H
#define MPI_HEAT_TRANSFER_LINEAR_PROGRAM_H

#include "files/config_file.h"
#include "m_matrix.h"

void linear_program(m_matrix<double> &matrix, ConfigFileOpt &config);

#endif //MPI_HEAT_TRANSFER_LINEAR_PROGRAM_H
