//
// Created by fenix on 5/31/20.
//

#include "mpi_program.h"
#include "m_matrix.h"
#include "visualization.h"


#define NONE            0
#define BEGIN_TAG       1                  /* message tag */
#define UPPER_TAG       2                  /* message tag */
#define LOWER_TAG       3                  /* message tag */
#define ITER_RES_TAG    4                  /* message tag */

static bool check_thermal_balance(const m_matrix<double> &field, const double &epsilon);

static void count_next_step_for_cell(const m_matrix<double> &previous, m_matrix<double> &current,
                                     const ConfigFileOpt &config, const int start, const int end);

void master_code(boost::mpi::communicator &world, const ConfigFileOpt &config) {
    const int workers_num = world.size() - 1;
    int offset = 0, work_block_width, upper_worker, lower_worker;
    std::cout << "Starting mpi_heat_transfer with " << workers_num << " worker tasks." << std::endl;;

    ///////////////////////////////////// Initialize grid //////////////////////////////////////
    std::cout << "Grid size: X= " << config.get_width() << "  Y= " << config.get_height() << std::endl;
    std::cout << "Initializing grid and writing initial.dat file..." << std::endl;
    m_matrix<double> main_matrix{config.get_field_filename()}; // load matrix
    ///////////////////////////////////// Initialize grid END //////////////////////////////////

    int main_work = static_cast<int>(config.get_height()) / workers_num;
    int extra_work = static_cast<int>(config.get_height()) % workers_num;

    ///////////////////////////////////// DISTRIBUTE WORK //////////////////////////////////////
    for (int dest_id = 1; dest_id <= workers_num; dest_id++) {
        work_block_width = (dest_id <= extra_work) ? main_work + 1 : main_work;
        // Tell each worker who its neighbors are, since they must exchange
        // data with each other.
        if (dest_id == 1)
            upper_worker = NONE;
        else
            upper_worker = dest_id - 1;
        if (dest_id == workers_num)
            lower_worker = NONE;
        else
            lower_worker = dest_id + 1;

        //  Now send startup information to each worker
        world.send(dest_id, BEGIN_TAG, offset);
        world.send(dest_id, BEGIN_TAG, work_block_width);
        world.send(dest_id, BEGIN_TAG, upper_worker);
        world.send(dest_id, BEGIN_TAG, lower_worker);
        world.send(dest_id, BEGIN_TAG, &main_matrix.get(static_cast<size_t>(offset), 0),
                   work_block_width * static_cast<int>(config.get_width()));
        std::cout << "Sent to task " << dest_id
                  << " rows= " << work_block_width
                  << " offset= " << offset << std::endl;
        std::cout << "upper_worker= " << upper_worker
                  << " lower_worker= " << lower_worker << std::endl;
        offset += work_block_width;
    }
    ///////////////////////////////////// DISTRIBUTE WORK END //////////////////////////////////

    ////////////////////////////////////// COLLECT RESULTS /////////////////////////////////////
    char name[100];
    size_t counter = 1;
    GifWriter gif_w{};
    GifBegin(&gif_w, "heatmap.gif-h", main_matrix.get_cols(), main_matrix.get_rows(), delay);
    size_t delay = 50;
    write_to_png("res/0.png", main_matrix, gif_w);
    while (!check_thermal_balance(main_matrix, config.get_epsilon())) {
        for (int source = 1; source <= workers_num; ++source) {
            world.send(source, ITER_RES_TAG, true);
            world.recv(source, ITER_RES_TAG, offset); // fail here
            world.recv(source, ITER_RES_TAG, work_block_width);
            world.recv(source, ITER_RES_TAG, &main_matrix.get(static_cast<size_t>(offset), 0),
                       work_block_width * static_cast<int>(config.get_width()));
        }
        std::cout << "SNAPSHOT " << counter << std::endl;
        sprintf(name, "res/%zu.png", counter++);
        write_to_png(name, main_matrix, gif_w);
    }
    for (int source = 1; source <= workers_num; source++) {
        world.send(source, ITER_RES_TAG, false);
    }
    GifEnd(&gif_w);
    std::cout << "############################## MAIN STOP #########################################" << std::endl;

    //////////////////////// COLLECT RESULTS END /////////////////////////////////
}

void slave_code(boost::mpi::communicator &world, const ConfigFileOpt &config) {
    int work_block_width, upper_worker, lower_worker, offset = 0;
    ///////////////////////////////////// INIT PARAMS /////////////////////////////////////
    world.recv(MASTER_ID, BEGIN_TAG, offset);
    world.recv(MASTER_ID, BEGIN_TAG, work_block_width);
    world.recv(MASTER_ID, BEGIN_TAG, upper_worker);
    world.recv(MASTER_ID, BEGIN_TAG, lower_worker);

    m_matrix<double> work_matrix_set[] = {
            m_matrix<double>{static_cast<size_t>(work_block_width) + 2, config.get_width()},
            m_matrix<double>{static_cast<size_t>(work_block_width) + 2, config.get_width()}};

    world.recv(MASTER_ID, BEGIN_TAG, &work_matrix_set[0].get(1, 0),
               work_block_width * static_cast<int>(config.get_width()));
    work_matrix_set[1] = work_matrix_set[0];

    /* Determine border elements.  Need to consider first and last columns. */
    /* Obviously, row 0 can't exchange with row 0-1.  Likewise, the last */
    /* row can't exchange with last+1.  */
    int start = 1;
    int end_i = work_block_width + 1;
    if (offset == 0)
        start = 2;
    if ((offset + work_block_width) == static_cast<int>(config.get_height()))
        end_i--;
    std::cout << "task=" << world.rank() << "  start=" << start + offset << "  end_i=" << end_i + offset
              << std::endl;
    ///////////////////////////////////// INIT PARAMS END //////////////////////////////////

    //////////////////////////////////// MAIN LOOP ////////////////////////////////////
    /* Begin doing iterations.  Must communicate border rows with                   //
     *  neighbors.  If I have the first or last grid row, then I only need          //
     *   to  communicate with one neighbor                                          */
    std::cout << "Task " << world.rank() << " received work. Beginning time steps..." << std::endl;
    uint8_t next_matrix = 0;
    size_t iter = 0;
    bool execute_flag;
    MPI_Status status{};
    while (true) {
        if (upper_worker != NONE) {
            MPI_Sendrecv(&work_matrix_set[next_matrix].get(1, 0), static_cast<int>(config.get_width()), MPI_DOUBLE,
                         upper_worker, LOWER_TAG, &work_matrix_set[next_matrix].get(0, 0),
                         static_cast<int>(config.get_width()),
                         MPI_DOUBLE, upper_worker, UPPER_TAG, MPI_COMM_WORLD, &status);
        }
        if (lower_worker != NONE) {
            MPI_Sendrecv(&work_matrix_set[next_matrix].get(static_cast<size_t>(work_block_width), 0),
                         static_cast<int>(config.get_width()), MPI_DOUBLE,
                         lower_worker, UPPER_TAG,
                         &work_matrix_set[next_matrix].get(static_cast<size_t>(work_block_width) + 1, 0),
                         static_cast<int>(config.get_width()),
                         MPI_DOUBLE, lower_worker, LOWER_TAG, MPI_COMM_WORLD, &status);
        }
        // Now call update to update the value of grid points
        count_next_step_for_cell(work_matrix_set[next_matrix], work_matrix_set[1 - next_matrix],
                                 config, start, end_i);
        if (++iter >= config.get_data_cycles()) {
            // Send my portion of results back to master
            world.recv(MASTER_ID, ITER_RES_TAG, execute_flag);
            if (!execute_flag)
                break;
            world.send(MASTER_ID, ITER_RES_TAG, offset);
            world.send(MASTER_ID, ITER_RES_TAG, work_block_width);
            world.send(MASTER_ID, ITER_RES_TAG, &work_matrix_set[next_matrix].get(1, 0),
                       work_block_width * static_cast<int>(config.get_width()));
            iter = 0;
        }
        next_matrix = 1 - next_matrix; // switch between next and current mantises
    }
    //////////////////////////////////// MAIN LOOP END ////////////////////////////////
}


static void count_next_step_for_cell(const m_matrix<double> &previous, m_matrix<double> &current,
                                     const ConfigFileOpt &config, const int start, const int end) {
    // count next area for the "current" field based on previous
    const int c = static_cast<int>(previous.get_cols() - 1);
    double x, y;
    for (size_t row = static_cast<size_t>(start); row < static_cast<size_t>(end); ++row) {
        for (size_t col = 1; col < static_cast<size_t>(c); ++col) {
            x = (previous.get(row - 1, col) - (2 * previous.get(row, col)) + previous.get(row + 1, col)) /
                std::pow(config.get_delta_x(), 2);
            y = (previous.get(row, col - 1) - (2 * previous.get(row, col)) + previous.get(row, col + 1)) /
                std::pow(config.get_delta_y(), 2);
            current.set(row, col, previous.get(row, col) + config.get_delta_t() * config.get_alpha() * (x + y));
        }
    }
}


static bool check_thermal_balance(const m_matrix<double> &field, const double &epsilon) {
    auto prev = &field.get(0, 0);
    for (int i = 0; i < static_cast<int>(field.get_rows() * field.get_cols()); ++i)
        if (std::abs(*prev - *(prev + i)) > epsilon)
            return false;
    return true;
}
