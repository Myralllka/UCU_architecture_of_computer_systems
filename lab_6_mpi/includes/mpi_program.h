//
// Created by fenix on 5/31/20.
//

#ifndef MPI_HEAT_TRANSFER_MPI_PROGRAM_H
#define MPI_HEAT_TRANSFER_MPI_PROGRAM_H

#include <files/config_file.h>
#include <boost/mpi.hpp>

#define MASTER_ID       0

void master_code(boost::mpi::communicator &world, const ConfigFileOpt &config);

void slave_code(boost::mpi::communicator &world, const ConfigFileOpt &config);


#endif //MPI_HEAT_TRANSFER_MPI_PROGRAM_H
