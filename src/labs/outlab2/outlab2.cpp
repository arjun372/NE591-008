
#include "../../utils/CommandLine.h"
#include "../../utils/Project.h"

struct Input {

};

struct Output {

};

class OutLab2 : public Project<Input, Output> {

public:
    OutLab2(CommandLineArgs args) : Project(args) {}

protected:

    HeaderInfo buildHeaderInfo() override {
        return HeaderInfo {
                .ProjectName = "OutLab 02",
                .ProjectDescription = "Lagrange Interpolation Polynomials",
                .SubmissionDate = "09/08/2023",
                .StudentName = "Arjun Earthperson",
        };
    }

    /**
 * @brief This function builds the input options for the program.
 *
 * @return A boost::program_options::options_description object containing the description of the input options.
 */
    void buildInputArguments(boost::program_options::options_description &values) override {
        values.add_options()
                ("scalar,k", boost::program_options::value<long double>(), "= scalar multiplier for [A]")
                ("m-rank,M", boost::program_options::value<long double>(), "= row rank for [A], row rank for [B]")
                ("n-rank,N", boost::program_options::value<long double>(), "= column rank for [B], row rank for [F]")
                ("j-rank,J", boost::program_options::value<long double>(), "= column rank for [F]");
    }



    void printInputArguments(boost::program_options::variables_map &values) override {
        std::cout<<values.count("");
    }

    void performInputArgumentsCheck(boost::program_options::variables_map &values) override {
        std::cout<<values.count("");
    }

    void buildInputs(Input &ToFill, boost::program_options::variables_map &values) override {
        std::cout<<values.count("");
    }

    void run(Output &output, Input &input, boost::program_options::variables_map &values) override {
        std::cout<<values.count("");
    }

};

/**
 * @brief The main function of the program.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 *
 * @return The exit status of the program.
 */
int main(int argc, char **argv) {
    CommandLineArgs args = {
            .argc = argc,
            .argv = argv,
    };
    auto labProject = OutLab2(args);
    labProject.execute();
}
