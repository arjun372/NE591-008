/**
* @file MPIProject.h
* @author Arjun Earthperson
* @date 10/13/2023
* @brief This file contains the definition of the MPIProject class, which is a template class for creating MPI projects.
 */

#ifndef NE591_008_MPIPROJECT_H
#define NE591_008_MPIPROJECT_H

#include "CommandLine.h"
#include "Project.h"
#include "utils/profiler/Profiler.h"
#include <boost/program_options.hpp>
#include <utility>

/**
 * @class MPIProject
 * @brief This class is a template for creating MPI projects. It provides methods for initializing MPI, getting inputs,
 * executing the project, and finalizing MPI.
 * @tparam InputType The type of the input data.
 * @tparam CommandLineParserType The type of the command line parser.
 * @tparam OutputType The type of the output data.
 */
template <typename InputType, typename CommandLineParserType, typename OutputType> class MPIProject {

    /**
     * @brief A static assertion to ensure that CommandLineParserType is a derived class of CommandLine.
     */
    static_assert(std::is_base_of<CommandLine<InputType>, CommandLineParserType>::value,
                  "CommandLineParserType must be a derived class of CommandLine");

  public:

    /**
     * @brief Constructor for the MPIProject class. Initializes MPI and gets the inputs for the project.
     * @param header The header information for the project.
     * @param args The command line arguments.
     */
    explicit MPIProject(HeaderInfo header, CommandLineArgs args) {
        MPI_Init(&args.argc, &args.argv);
        MPI_Comm_size(MPI_COMM_WORLD, &this->_size);
        MPI_Comm_rank(MPI_COMM_WORLD, &this->_rank);
        _uuid = "["+hostname()+"]["+std::to_string(this->_rank)+"]: ";
        _inputs = getInputs(header, args, this->_rank, this->_size);
    }

    /**
     * @brief Destructor for the MPIProject class.
     */
    virtual ~MPIProject() = default;

  protected:
    /**
     * @brief The rank of the current process in the MPI_COMM_WORLD communicator.
     */
    int _rank = -1;
    /**
     * @brief The total number of processes in the MPI_COMM_WORLD communicator.
     */
    int _size = 1;
    /**
     * @brief A unique identifier for the current process.
     */
    std::string _uuid;
    /**
     * @brief The inputs for the project.
     */
    InputType _inputs;
    /**
     * @brief The outputs of the project.
     */
    OutputType _outputs;

    /**
     * @brief Gets the inputs for the project. If the rank is not 0, it receives the inputs from the main process. If
     * the rank is 0, it sends the inputs to all other processes.
     * @param header The header information for the project.
     * @param args The command line arguments.
     * @param rank The rank of the current process.
     * @param size The total number of processes.
     * @return The inputs for the project.
     */
    static InputType getInputs(HeaderInfo header, CommandLineArgs args, const int rank, const int size) {
        // receive from main
        if (rank != 0) {
            return blockingReceive<InputType>();
        }

        // send to all but main
        InputType inputs;
        auto terminal = CommandLineParserType(header, args);
        inputs = terminal.getInputs();
        for(int i = 1; i < size; i++) {
            blockingSend(inputs, i);
        }
        return inputs;
    }

    /**
     * @brief Creates a Profiler object with the given function, number of samples, timeout, and description.
     * @tparam Func The type of the function to be profiled.
     * @param func The function to be profiled.
     * @param samples The number of samples to be taken.
     * @param timeout The maximum time for the profiling.
     * @param description The description of the profiling.
     * @return A Profiler object.
     */
    template <typename Func>
    auto getProfiler(Func func, size_t samples, long double timeout = 0, std::string description = "") {
        return Profiler(func, samples, timeout, description);
    }

  public:
    /**
     * @brief Executes the project. It calls the preRun, run, and postRun methods, and then finalizes MPI.
     */
    void execute() {
        if (preRun(this->_outputs, this->_inputs, this->_rank, this->_size)) {
            goto quit;
        }

        if (run(this->_outputs, this->_inputs, this->_rank, this->_size)) {
            goto quit;
        }

        if (postRun(this->_outputs, this->_inputs, this->_rank, this->_size)) {
            goto quit;
        }
    quit:
        finalize();
    }

  protected:
    /**
     * @brief Finalizes MPI.
     */
    void finalize() {
        MPI_Finalize();
    }
    /**
     * @brief A virtual method that is called before the run method. It can be overridden in derived classes.
     * @param output The output of the project.
     * @param input The input of the project.
     * @param rank The rank of the current process.
     * @param size The total number of processes.
     * @return A boolean value indicating whether to quit the execution.
     */
    virtual bool preRun(OutputType &output, InputType &input, const int rank, const int size) {return false;}
    /**
     * @brief A virtual method that is called to run the project. It can be overridden in derived classes.
     * @param output The output of the project.
     * @param input The input of the project.
     * @param rank The rank of the current process.
     * @param size The total number of processes.
     * @return A boolean value indicating whether to quit the execution.
     */
    virtual bool run(OutputType &output, InputType &input, const int rank, const int size) {return false;}
    /**
     * @brief A virtual method that is called after the run method. It can be overridden in derived classes.
     * @param output The output of the project.
     * @param input The input of the project.
     * @param rank The rank of the current process.
     * @param size The total number of processes.
     * @return A boolean value indicating whether to quit the execution.
     */
    virtual bool postRun(OutputType &output, InputType &input, const int rank, const int size) {return false;}

  private:

    /**
     * @brief Sends data to a process with a given rank.
     * @tparam DataType The type of the data to be sent.
     * @param data The data to be sent.
     * @param rank The rank of the process to send the data to.
     */
    template <typename DataType>
    static void blockingSend(const DataType &data, const int rank) {
        std::ostringstream oss;
        boost::archive::text_oarchive oa(oss);
        oa << data;
        std::string serialized = oss.str();
        MPI_Send(serialized.c_str(), serialized.size() + 1, MPI_CHAR, rank, 0, MPI_COMM_WORLD);
    }

    /**
     * @brief Receives data from any process.
     * @tparam DataType The type of the data to be received.
     * @return The received data.
     */
    template <typename DataType>
    static DataType blockingReceive() {
        MPI_Status status;
        int length;
        MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHAR, &length);
        char* buffer = new char[length];
        MPI_Recv(buffer, length, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::string received_str(buffer);
        delete[] buffer;
        std::istringstream iss(received_str);
        boost::archive::text_iarchive ia(iss);
        DataType received;
        ia >> received;
        return received;
    }

  public:
    /**
     * @brief Gets the name of the processor that the current process is running on.
     * @return The name of the processor.
     */
    static std::string hostname() {
        char hostname[MPI_MAX_PROCESSOR_NAME];
        int resultlen;
        MPI_Get_processor_name(hostname, &resultlen);
        return std::string{hostname, static_cast<size_t>(resultlen)};
    }
};

#endif // NE591_008_MPIPROJECT_H
