/*
 * Arjun Earthperson
 * Commandline parsing helper functions for lab projects in NE591-008.
*/

#pragma once

#include <string>
#include <boost/program_options.hpp>

#include "Helpers.h"
#include "project-config.h"

typedef struct  {
    std::string ProjectName;
    std::string ProjectDescription;
    std::string SubmissionDate;
    std::string StudentName;
} HeaderInfo;

class CommandLine {

public:
    explicit CommandLine(HeaderInfo &headerInfo, boost::program_options::options_description &inputs, int argc, char** argv) {

        printHeader(headerInfo);

        boost::program_options::options_description options("");
        options.add(inputs);
        options.add(buildGenerics());
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), variablesMap);
        boost::program_options::notify(variablesMap);

        if (variablesMap.count("help")) {
            std::cout << options << "\n";
            exit(0);
        }

        initialize(options);
    }

    boost::program_options::variables_map &getArguments() {
        return variablesMap;
    }

    static void printLine() {
        std::cout << R"(--------------------------------------------------------------------------------)"<<"\n";
    }

private:
    boost::program_options::options_description optionsDescription;
    boost::program_options::variables_map variablesMap;

    static boost::program_options::options_description buildGenerics() {
        boost::program_options::options_description generics("General options");
        generics.add_options()
                ("help,h", "= Show this help message")
                ("quiet,q", "= Reduce verbosity")
                ("precision,p", boost::program_options::value<int>()->default_value(max_precision), "= Number of digits to represent double_t")
                ;
        return generics;
    }

    static void printHeader(HeaderInfo &headerInfo) {
        std::cout << R"(
                    ███╗   ██╗███████╗      ███████╗ █████╗  ██╗
                    ████╗  ██║██╔════╝      ██╔════╝██╔══██╗███║
                    ██╔██╗ ██║█████╗  █████╗███████╗╚██████║╚██║
                    ██║╚██╗██║██╔══╝  ╚════╝╚════██║ ╚═══██║ ██║
                    ██║ ╚████║███████╗      ███████║ █████╔╝ ██║
                    ╚═╝  ╚═══╝╚══════╝      ╚══════╝ ╚════╝  ╚═╝
        )" << "\n";
        printLine();
        std::cout << "\t" << headerInfo.ProjectName << ": " << headerInfo.ProjectDescription << "\n";
        std::cout << "\t" << headerInfo.StudentName << "\n";
        std::cout << "\t" << headerInfo.SubmissionDate << "\n";
        printLine();
    }

    void printPrecisionInformation() {
        std::cout << "Precision in digits:  ";
        std::cout << "default: " << default_precision <<", ";
        std::cout << "maximum: " << max_precision <<", ";
        std::cout << "current: " << variablesMap["precision"].as<int>()<<"\n";
        printLine();
    }

    void initialize(boost::program_options::options_description &options) {

        if (variablesMap.count("quiet")) {
            return;
        }

        printCompileConfigs();

        // print command line options
        std::cout << options << "\n";
        printLine();

        printPrecisionInformation();
    }

    static void printCompileConfigs() {
        std::cout<<"Build Configuration\n";
        std::cout<<"compiler: "<<CXX_COMPILER_ID<<" "<<CXX_COMPILER_VERSION<<"\n";
        std::cout<<"flags: "<<CXX_FLAGS<<"\n";
        std::cout<<"Boost: "<<Boost_VERSION<<Boost_LIBRARIES<<"\n";
        printLine();
    }
};
