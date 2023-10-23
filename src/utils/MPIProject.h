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
template <typename InputType, typename CommandLineParserType, typename OutputType> class MPIProject
: public Project<InputType, CommandLineParserType, OutputType> {

  public:
    explicit MPIProject(CommandLineArgs args) : Project<InputType, CommandLineParserType, OutputType>(args) {
        std::cout<<"running constructor now: "<<this->rank<<"\n";
        MPI_Init(&args.argc, &args.argv);
        MPI_Comm_size(MPI_COMM_WORLD, &this->size);
        MPI_Comm_rank(MPI_COMM_WORLD, &this->rank);
        uuid = "["+getHostName()+"]["+std::to_string(this->rank)+"]: ";
    }

  protected:
    int rank = -1;
    int size = 1;
    std::string uuid;
    bool mpi_initialized = false;
    //char hostname[MPI_MAX_PROCESSOR_NAME];


    // TODO:: DOCUMENT
    void initialize() override {
        InputType inputs;
        // if rank leader, collect all inputs, and then broadcast and sync variables with all ranks.
        if(this->rank == 0) {
            Project<InputType, CommandLineParserType, OutputType>::initialize();
            inputs = getTerminal().getInputs();
            for(int i = 1; i < size; i++) {
                inputs.n = i;
                sendInputs(inputs, i);
            }
            inputs.n = 0;
        } else {
            inputs = receiveInputs();
        }
        std::cout<<uuid<<"inputs: "<<inputs<<"\n";
    }

  public:
    void execute() override {
        initialize();
        finalize();
    }

    CommandLineParserType getTerminal() const override {
        return Project<InputType, CommandLineParserType, OutputType>::getTerminal();
    }

  protected:
    void finalize() override {
        std::cout<<uuid<<"finalizing\n";
        MPI_Finalize();
        mpi_initialized = false;
    }
    void timedRun() override {

    }
    void preRun(OutputType &output, InputType &input, boost::program_options::variables_map &values) override {

    }
    void run(OutputType &output, InputType &input, boost::program_options::variables_map &values) override {

    }
    void postRun(OutputType &output, InputType &input, boost::program_options::variables_map &values) override {

    }

  private:
    static void sendInputs(const InputType &inputs, const int rank) {
        std::ostringstream oss;
        boost::archive::text_oarchive oa(oss);
        oa << inputs;
        std::string serialized_str = oss.str();
        MPI_Send(serialized_str.c_str(), serialized_str.size() + 1, MPI_CHAR, rank, 0, MPI_COMM_WORLD);
    }

    // TODO:: DOCUMENT
    static InputType receiveInputs() {
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
        InputType received_input;
        ia >> received_input;
        return received_input;
    }

  public:
    ~MPIProject() override {
        std::cout<<uuid<<"running destructor\n";
    }

    // TODO:: DOCUMENT
    static std::string getHostName() {
        char hostname[MPI_MAX_PROCESSOR_NAME];
        int resultlen;
        MPI_Get_processor_name(hostname, &resultlen);
        return std::string{hostname, static_cast<size_t>(resultlen)};
    }
};

#endif // NE591_008_MPIPROJECT_H
