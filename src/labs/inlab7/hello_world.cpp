/**
 * @file hello_world.cpp
 * @author Arjun Earthperson
 * @date 10/06/2023
 * @brief This program demonstrates the use of MPI for parallel computing.
 * It reads a name from an input file and each process says hello to the name.
 * It also measures and prints the execution time.
 */

// Include necessary libraries
#include <iostream> // For input/output operations
#include <chrono>   // For time-related operations
#include <fstream>  // For file handling operations
#include <mpi.h>    // For MPI (Message Passing Interface) operations

/**
 * @brief main function
 * @param argc Number of command-line arguments
 * @param argv Array of command-line arguments
 * @return int Returns 0 upon successful execution
 */
int main(int argc, char* argv[]) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int numprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    // Get the current process ID (rank)
    int rank_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);

    // Get the name of the processor
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int resultlen;
    MPI_Get_processor_name(hostname, &resultlen);


    // Open the input file
    std::ifstream input("input");

    // Check if the file was opened successfully
    if (!input) {
        std::cerr << "Unable to open file";
        exit(1); // Terminate with error
    }

    // Read the name from the input file
    std::string name;
    input >> name;

    // Close the file after reading
    input.close();

    // If the rank is 0, print the initial message
    if(rank_id == 0) {
        std::cout<<R"(
                 ███╗   ██╗███████╗      ███████╗ █████╗  ██╗
                 ████╗  ██║██╔════╝      ██╔════╝██╔══██╗███║
                 ██╔██╗ ██║█████╗  █████╗███████╗╚██████║╚██║
                 ██║╚██╗██║██╔══╝  ╚════╝╚════██║ ╚═══██║ ██║
                 ██║ ╚████║███████╗      ███████║ █████╔╝ ██║
                 ╚═╝  ╚═══╝╚══════╝      ╚══════╝ ╚════╝  ╚═╝
)";
        std::cout<<R"(================================================================================)"<<std::endl;
        std::cout<<"InLab 07: MPI Parallel Hello World\nArjun Earthperson\n10/06/2023"<<std::endl;
        std::cout<<R"(================================================================================)"<<std::endl;
        std::cout<<R"(                             Hello parallel worlds!                             )"<<std::endl;
        std::cout<<R"(--------------------------------------------------------------------------------)"<<std::endl;
    }

    // Each process says hello
    std::cout<<"Hello "<<name<<" from MPI Task "<<rank_id<<" on host "<<hostname<<std::endl;

    // Measure the execution time
    auto start = std::chrono::high_resolution_clock::now();
    // ... (the code to be measured goes here) ...
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    // Print the execution time
    std::cout<<"["<<hostname<<"]"<<"["<<rank_id<<"]: "<<"Execution time (sec) = "<<elapsed.count()<<std::endl;

    // We can also write to standard error
    std::cerr<<"["<<hostname<<"]"<<"["<<rank_id<<"]: "<<"I log my errors here"<<std::endl;
    std::cout<<R"(--------------------------------------------------------------------------------)"<<std::endl;

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
