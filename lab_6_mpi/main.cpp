#include <iostream>
#include <string>
#include <boost/mpi.hpp>
#include <filesystem>

#include "files/config_file.h"
#include "m_matrix.h"
#include "linear_program.h"

#define MASTER_ID 0
#define NONE 0
#define BEGIN_TAG       1                  /* message tag */
#define UPPER_TAG       2                  /* message tag */
#define LOWER_TAG       3                  /* message tag */
#define CONTINUE_TAG    4                  /* message tag */
#define ITER_RES_TAG    5                  /* message tag */

ConfigFileOpt parse_args(int argc, char **argv);

void assert_valid_config(const ConfigFileOpt &conf);

int main(int argc, char *argv[]) {
    ConfigFileOpt config = parse_args(argc, argv);
    assert_valid_config(config);
    ////////////////////////////////////////////////
    m_matrix<double> tmp(config.get_field_filename()); // load matrix
    linear_program(tmp, config);
    ////////////////////////////////////////////////
    //
    // TODO: Add Ctrl+C
    // TODO: Add stop after thermal balance
    //
    ////////////////////////////////////////////////
//    boost::mpi::environment env{argc, argv};
//    boost::mpi::communicator world{};
//    int workers_num = world.size() - 1, upper_worker, lower_worker;
//    size_t offset = 0;
//    int work_block_width;
//
//    ////////////////////////////////////////////// MASTER  CODE ////////////////////////////////////////////////////////
//    if (world.rank() == MASTER_ID) {
//        if (workers_num) {
//            std::cerr << "ERROR: the number of tasks must be at least 1.\nQuitting..." << std::endl;
//            boost::mpi::environment::abort(1);
//            exit(1);
//        }
//        std::cout << "Starting mpi_heat_transfer with " << workers_num << " worker tasks." << std::endl;;
//
//        ///////////////////////////////////// Initialize grid //////////////////////////////////////
//        std::cout << "Grid size: X= " << config.get_width() << "  Y= " << config.get_height() << "  Time steps= " << 0
//                  << std::endl;
//        std::cout << "Initializing grid and writing initial.dat file..." << std::endl;
//        m_matrix<double> u{config.get_field_filename()}; // load matrix
//        ///////////////////////////////////// Initialize grid END //////////////////////////////////
//
//        int main_work = static_cast<int>(config.get_height() / workers_num);
//        int extra_work = static_cast<int>(config.get_height() % workers_num);
//
//        ///////////////////////////////////// DISTRIBUTE WORK //////////////////////////////////////
//        for (int dest_id = 1; dest_id <= workers_num; dest_id++) {
//            work_block_width = (dest_id <= extra_work) ? main_work + 1 : main_work;
//            /* Tell each worker who its neighbors are, since they must exchange */
//            /* data with each other. */
//            if (dest_id == 1)
//                upper_worker = NONE;
//            else
//                upper_worker = dest_id - 1;
//            if (dest_id == workers_num)
//                lower_worker = NONE;
//            else
//                lower_worker = dest_id + 1;
//
//            /*  Now send startup information to each worker  */
//            world.send(dest_id, BEGIN_TAG, offset);
//            world.send(dest_id, BEGIN_TAG, work_block_width);
//            world.send(dest_id, BEGIN_TAG, upper_worker);
//            world.send(dest_id, BEGIN_TAG, lower_worker);
//            world.send(dest_id, BEGIN_TAG, &u.get(offset, 0),
//                       static_cast<int>(work_block_width * config.get_width())); // TODO : set valid pointer
//            std::cout << "Sent to task " << dest_id << " rows= " << work_block_width << " offset= " << offset
//                      << std::endl;
//            std::cout << "upper_worker= " << upper_worker << " lower_worker= " << lower_worker << std::endl;
//            offset = offset + work_block_width;
//        }
//        ///////////////////////////////////// DISTRIBUTE WORK END //////////////////////////////////
//
//        //////////////////////// COLLECT RESULTS /////////////////////////////////////
//        size_t iters = 0;
//        while (!check_thermal_balance(u)) {
//            for (size_t source = 1; source <= workers_num; source++) {
//                world.recv(source, ITER_RES_TAG, offset);
//                world.recv(source, ITER_RES_TAG, work_block_width);
//                world.recv(source, ITER_RES_TAG, &u.get(offset, 0),
//                           static_cast<int>(work_block_width * config.get_width()));
//            }
//            if (++iters > config.get_data_cycles()) {
//                // TODO: add VISUALIZATION
//            }
//        }
//        //////////////////////// COLLECT RESULTS END /////////////////////////////////
//    }
//    /////////////////////////////////////////////// MASTER CODE END ////////////////////////////////////////////////////
//
//
//    /////////////////////////////////////////////// SLAVE CODE /////////////////////////////////////////////////////////
//    if (world.rank() != MASTER_ID) {
//        ///////////////////////////////////// INIT PARAMS /////////////////////////////////////
//        world.recv(MASTER_ID, BEGIN_TAG, offset);
//        world.recv(MASTER_ID, BEGIN_TAG, work_block_width);
//        world.recv(MASTER_ID, BEGIN_TAG, upper_worker);
//        world.recv(MASTER_ID, BEGIN_TAG, lower_worker);
//
//        m_matrix<double> u[] = {m_matrix<double>{static_cast<size_t>(work_block_width + 2), config.get_width()},
//                                m_matrix<double>{static_cast<size_t>(work_block_width + 2), config.get_width()}};
//        world.recv(MASTER_ID, BEGIN_TAG, &u[0].get(1, 0), static_cast<int>(work_block_width * config.get_width()));
//
//        /* Determine border elements.  Need to consider first and last columns. */
//        /* Obviously, row 0 can't exchange with row 0-1.  Likewise, the last */
//        /* row can't exchange with last+1.  */
//        size_t start = 0;
//        size_t end = work_block_width - 1;
//        if (offset == 0)
//            start = 1;
//        if ((offset + work_block_width) == config.get_height())
//            end--;
//        std::cout << "task=" << world.rank() << "  start=" << start + offset << "  end=" << end + offset << std::endl;
//        ///////////////////////////////////// INIT PARAMS END //////////////////////////////////
//
//        //////////////////////////////////// MAIN LOOP ////////////////////////////////////
//        /* Begin doing STEPS iterations.  Must communicate border rows with */
//        /* neighbors.  If I have the first or last grid row, then I only need */
//        /*  to  communicate with one neighbor  */
//        std::cout << "Task " << world.rank() << " received work. Beginning time steps..." << std::endl;
//        uint8_t next_matrix = 0;
//        while (true) {
//            if (upper_worker != NONE) { // TODO: set valid offsets in gets
//                world.send(upper_worker, LOWER_TAG, &u[next_matrix].get(1, 0), config.get_width());
//                world.recv(upper_worker, UPPER_TAG, &u[next_matrix].get(0, 0), config.get_width());
//            }
//            if (lower_worker != NONE) {
//                world.send(lower_worker, UPPER_TAG, &u[next_matrix].get(workers_num - 1, 0), config.get_width());
//                world.recv(lower_worker, LOWER_TAG, &u[next_matrix].get(workers_num, 0), config.get_width());
//            }
//            /* Now call update to update the value of grid points */
//            // TODO: add updating && ommiting update of borders
////            update(start, end, config.get_width(), &u[next_matrix].get(0, 0 ), &u[1 - next_matrix].get(0, 0 ));
//
//            /* Send my portion of results back to master */
//            world.send(MASTER_ID, ITER_RES_TAG, offset);
//            world.send(MASTER_ID, ITER_RES_TAG, work_block_width);
//            world.send(MASTER_ID, ITER_RES_TAG, &u[next_matrix].get(1, 0), work_block_width * config.get_width());
//
//            next_matrix = 1 - next_matrix;
//        }
//        //////////////////////////////////// MAIN LOOP END ////////////////////////////////
//    }
//    /////////////////////////////////////////////// SLAVE CODE END /////////////////////////////////////////////////////
//
//    std::cout << "FINISH process with rank " << world.rank() << " of " << world.size() << std::endl;
//    return 0;
}

ConfigFileOpt parse_args(int argc, char **argv) {
    //  ##################### Program Parameter Parsing ######################
    std::string filename = "config.conf";
    if (argc == 2) {
        filename = argv[1];
    } else if (argc > 2) {
        std::cerr << "Too many arguments. Usage: \n"
                     "\tprogram [config-filename]\n" << std::endl;
        exit(1);
    }
    //  #####################    Config File Parsing    ######################
    ConfigFileOpt config;
    try {
        config.parse(filename);
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        exit(3);
    }
    return config;
}

void assert_valid_config(const ConfigFileOpt &config) {
    // TODO: rewrite through exceptions
    if (!std::filesystem::exists(config.get_field_filename())) {
        std::cerr << "Error: File or Directory '" << config.get_field_filename()
                  << "' do not exist (or can not be created)!"
                  << std::endl;
        exit(21);
    } else if (config.get_field_filename().empty()) {
        std::cerr << "Error: Field file is empty or missing field file filename!" << std::endl;
        exit(23);
    } else if (config.get_delta_t() >=
               std::pow(std::max(config.get_delta_x(), config.get_delta_y()), 2) / config.get_alpha() / 4) {
        std::cerr << "Violation of the Von Neumann criteria for input data." << std::endl;
        exit(23);
    }
}
