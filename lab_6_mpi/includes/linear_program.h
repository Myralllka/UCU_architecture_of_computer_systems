//
// Created by myralllka on 5/28/20.
//

#ifndef MPI_HEAT_TRANSFER_LINEAR_PROGRAM_H
#define MPI_HEAT_TRANSFER_LINEAR_PROGRAM_H

#include <sstream>
#include "files/config_file.h"
#include "m_matrix.h"
#include "code_controle.h"

void linear_program(m_matrix<double> matrix, const ConfigFileOpt &config);

bool check_thermal_balance(m_matrix<double> &field);

#endif //MPI_HEAT_TRANSFER_LINEAR_PROGRAM_H
