#include <iostream>

#include <boost/program_options.hpp>

#include "utils/CommandLine.h"

int main(int argc, char** argv) {


    HeaderInfo programInfo;
    programInfo.ProjectName = "InLab 01";
    programInfo.StudentName = "Arjun Earthperson";
    programInfo.SubmissionDate = "08/30/2023";
    programInfo.ProjectDescription = "Taylor series approximation";

    boost::program_options::options_description arguments("Input variables");
    arguments.add_options()
            ("angle,x", boost::program_options::value<double_t>(), "= Angle x such that [abs(x) < 1.0]")
            ("error-threshold,t", boost::program_options::value<double_t>(), "= Iterative error threshold [e > 0]")
            ("iterations,n", boost::program_options::value<size_t>(), "= Total number of iterations")
            ;
    boost::program_options::variables_map values = CommandLine(programInfo, arguments, argc, argv).getArguments();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
