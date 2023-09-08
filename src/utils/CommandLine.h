/**
 * @file CommandLine.h
 * @brief This file contains the definition of the CommandLine class and the HeaderInfo struct.
 * @author Arjun Earthperson
 * @date 08/30/2023
 * The CommandLine class is used to parse command line arguments and display information about the project.
 * The HeaderInfo struct is used to store information about the project, such as the project name, description,
 * submission date, and student name.
 */

#pragma once

#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include "project-config.h"

/**
 * @struct HeaderInfo
 * @brief A struct to store information about the project.
 *
 * This struct contains four members: ProjectName, ProjectDescription, SubmissionDate, and StudentName.
 * These members are used to store the project name, project description, submission date, and student name respectively.
 */
typedef struct  {
    std::string ProjectName;
    std::string ProjectDescription;
    std::string SubmissionDate;
    std::string StudentName;
} HeaderInfo;

struct CommandLineArgs {
    int argc{};
    char **argv{};
};


const auto default_precision {std::cout.precision()}; // Default precision for output streams
constexpr auto max_precision {std::numeric_limits<long double>::digits10 + 1}; // Maximum precision for long double type

/**
 * @class CommandLine
 * @brief A class to parse command line arguments and display information about the project.
 *
 * This class contains methods to parse command line arguments using the Boost library, and to display information about the project.
 * The constructor takes a HeaderInfo object and command line arguments, and initializes the class.
 * The getArguments method returns the parsed command line arguments.
 * The printLine method prints a line to the console.
 * The buildGenerics method builds a set of generic command line options.
 * The printHeader method prints the project header to the console.
 * The printPrecisionInformation method prints information about the precision of long double values.
 * The initialize method initializes the class by printing the compile configurations and precision information, and parsing the command line arguments.
 * The printCompileConfigs method prints the compile configurations to the console.
 */

template <typename InputType> class CommandLine {

public:

    // TODO rewrite
    explicit CommandLine(HeaderInfo headerInfo, CommandLineArgs args) {
        cmdArgs = args;
        initialized = false;
        printHeader(headerInfo);
    }

    explicit CommandLine() {
        variablesMap = boost::program_options::variables_map();
    }

    virtual ~CommandLine() = default;

    /**
     * @fn boost::program_options::variables_map &CommandLine::getArguments()
     * @brief Returns the parsed command line arguments.
     *
     * This method returns a reference to the variables map that contains the parsed command line arguments.
     */
    boost::program_options::variables_map &getArguments() {
        if(!initialized) {
            initialize();
        }
        return variablesMap;
    }

    int getCurrentPrecision() {
        return variablesMap["precision"].as<int>();
    }

    InputType &getInputs() {
        if(!initialized) {
            initialize();
        }
        return inputs;
    }

    /**
     * @fn void CommandLine::printLine()
     * @brief Prints a line to the console.
     *
     * This method prints a line of dashes to the console.
     */
    static void printLine() {
        std::cout << R"(--------------------------------------------------------------------------------)"<<"\n";
    }

protected:
    // Handle input arguments
    virtual void buildInputArguments(boost::program_options::options_description &inputArguments) = 0;
    virtual void printInputArguments(boost::program_options::variables_map &values) = 0;
    virtual void performInputArgumentsCheck(boost::program_options::variables_map &values) = 0;
    // fill inputs object based on values
    virtual void buildInputs(InputType &ToFill, boost::program_options::variables_map &values) = 0;

private:
    boost::program_options::variables_map variablesMap;
    bool initialized = false;
    CommandLineArgs cmdArgs;
    InputType inputs = InputType();

    /**
     * @fn boost::program_options::options_description CommandLine::buildGenerics()
     * @brief Builds a set of generic command line options.
     *
     * This method builds a set of generic command line options, such as "help", "quiet", and "precision", and returns them as an options description.
     */
    static boost::program_options::options_description buildGenerics() {
        boost::program_options::options_description generics("General options");
        generics.add_options()
                ("help,h", "= Show this help message")
                ("quiet,q", "= Reduce verbosity")
                ("precision,p", boost::program_options::value<int>()->default_value(15), "= Number of digits to represent long double")
                ("profile,P", "= Turn on profiling for performance comparison")
                ;
        return generics;
    }

    /**
     * @fn void CommandLine::printHeader(HeaderInfo &headerInfo)
     * @brief Prints the project header to the console.
     *
     * This method prints the project name, project description, student name, and submission date to the console.
     */
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

    /**
     * @fn void CommandLine::printPrecisionInformation()
     * @brief Prints information about the precision of long double values.
     *
     * This method prints the default, maximum, and current precision of long double values to the console.
     */
    void printPrecisionInformation() {
        std::cout << "Precision in digits:  ";
        std::cout << "default: " << default_precision <<", ";
        std::cout << "maximum: " << max_precision <<", ";
        std::cout << "current: " << variablesMap["precision"].as<int>()<<"\n";
        printLine();
    }


    void initialize() {

        if(initialized) {
            return;
        }

        initialized = true;
        boost::program_options::options_description options("Parameters");
        buildInputArguments(options);
        options.add(buildGenerics());
        boost::program_options::store(boost::program_options::parse_command_line(cmdArgs.argc, cmdArgs.argv, options), variablesMap);
        boost::program_options::notify(variablesMap);

        //if help, print options and exit
        if (variablesMap.count("help")) {
            std::cout << options << "\n";
            exit(0);
        }

        // print compiler information
        if (!variablesMap.count("quiet")) {
            printCompileConfigs();
            // print command line options
            std::cout << options << "\n";
            printLine();
            printPrecisionInformation();
        }

        // consume and correct the input arguments
        performInputArgumentsCheck(variablesMap);
        // print the input arguments
        printInputArguments(variablesMap);
        // finally, build and save the inputs objet
        buildInputs(inputs, variablesMap);
    }

    /**
     * @fn void CommandLine::printCompileConfigs()
     * @brief Prints the compile configurations to the console.
     *
     * This method prints the compiler ID, compiler version, compiler flags, and Boost version and libraries to the
     * console.
     */
    static void printCompileConfigs() {
        std::cout<<"Build Configuration\n";
        std::cout<<"compiler: "<<CXX_COMPILER_ID<<" "<<CXX_COMPILER_VERSION<<"\n";
        std::cout<<"flags: "<<CXX_FLAGS<<"\n";
        std::cout<<"Boost: "<<Boost_VERSION<<"; "<<Boost_LIBRARIES<<"\n";
        printLine();
    }
};
