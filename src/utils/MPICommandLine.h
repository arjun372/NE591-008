/**
* @file MPICommandLine.h
* @brief TODO:: DOCUMENT
* @author Arjun Earthperson
* @date 10/13/2023
*/

#ifndef NE591_008_MPICOMMANDLINE_H
#define NE591_008_MPICOMMANDLINE_H

#include "CommandLine.h"
#include <mpi.h>

template <typename InputType> class MPICommandLine : public CommandLine<InputType> {

  public:
    MPICommandLine() = default;
    MPICommandLine(const HeaderInfo &headerInfo, const CommandLineArgs &args) : CommandLine<InputType>(headerInfo, args) {}

//  protected:
//    int rank = -1;
//    int size = 1;
//    bool mpi_initialized = false;
//
//    void initialize() override {
//
//        if (mpi_initialized) {
//            return;
//        }
//
//        mpi_initialized = true;
//        MPI_Init(&CommandLine<InputType>::cmdArgs.argc, &CommandLine<InputType>::cmdArgs.argv);
//        MPI_Comm_size(MPI_COMM_WORLD, &this->size);
//        MPI_Comm_rank(MPI_COMM_WORLD, &this->rank);
//
//        if(this->rank == 0) {
//            CommandLine<InputType>::initialize();
//           // exit(0);
//        } else {
//           // exit(0);
//        }
////        // CommandLine::initialize();
////        int numprocs, rank_id;
////        MPI_Status stat;
////
////        MPI_Init(&args.argc, &args.argv);
////        MPI_Comm_size(MPI_COMM_WORLD, numprocs);
////        MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
////        //
////        auto n = 1000;
////        MPI_Bcast(&n, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
//
//    }
//
};

#endif // NE591_008_MPICOMMANDLINE_H
