/**
* @file MPIProject.h
* @author Arjun Earthperson
* @date 10/13/2023
* @brief TODO:: DOCUMENT
*/

#ifndef NE591_008_MPIPROJECT_H
#define NE591_008_MPIPROJECT_H

#include "CommandLine.h"
#include "Profiler.h"
#include "Project.h"
#include <boost/program_options.hpp>
#include <utility>

/**
 * @class MPIProject
 * @brief TODO:: DOCUMENT
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

    // TODO:: DOCUMENT
    explicit MPIProject(HeaderInfo header, CommandLineArgs args) {
        MPI_Init(&args.argc, &args.argv);
        MPI_Comm_size(MPI_COMM_WORLD, &this->_size);
        MPI_Comm_rank(MPI_COMM_WORLD, &this->_rank);
        _uuid = "["+hostname()+"]["+std::to_string(this->_rank)+"]: ";
        _inputs = getInputs(header, args, this->_rank, this->_size);
    }

    // TODO:: DOCUMENT
    virtual ~MPIProject() {}

  protected:
    // TODO:: DOCUMENT
    int _rank = -1;
    // TODO:: DOCUMENT
    int _size = 1;
    // TODO:: DOCUMENT
    std::string _uuid;
    // TODO:: DOCUMENT
    InputType _inputs;
    // TODO:: DOCUMENT
    OutputType _outputs;

    // TODO:: DOCUMENT
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
    // TODO:: DOCUMENT
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
    // TODO:: DOCUMENT
    void finalize() {
        MPI_Finalize();
    }
    // TODO:: DOCUMENT
    virtual bool preRun(OutputType &output, InputType &input, const int rank, const int size) {return false;}
    // TODO:: DOCUMENT
    virtual bool run(OutputType &output, InputType &input, const int rank, const int size) {return false;}
    // TODO:: DOCUMENT
    virtual bool postRun(OutputType &output, InputType &input, const int rank, const int size) {return false;}

  private:

    // TODO:: DOCUMENT
    template <typename DataType>
    static void blockingSend(const DataType &data, const int rank) {
        std::ostringstream oss;
        boost::archive::text_oarchive oa(oss);
        oa << data;
        std::string serialized = oss.str();
        MPI_Send(serialized.c_str(), serialized.size() + 1, MPI_CHAR, rank, 0, MPI_COMM_WORLD);
    }

    // TODO:: DOCUMENT
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
    // TODO:: DOCUMENT
    static std::string hostname() {
        char hostname[MPI_MAX_PROCESSOR_NAME];
        int resultlen;
        MPI_Get_processor_name(hostname, &resultlen);
        return std::string{hostname, static_cast<size_t>(resultlen)};
    }
};

#endif // NE591_008_MPIPROJECT_H
