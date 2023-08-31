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
#include <boost/program_options.hpp>

#include "Helpers.h"
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
class CommandLine {

public:
    /**
     * @fn CommandLine::CommandLine(HeaderInfo &headerInfo, boost::program_options::options_description &inputs, int argc, char** argv)
     * @brief The constructor for the CommandLine class.
     *
     * This constructor takes a HeaderInfo object and command line arguments, and initializes the class.
     * It first prints the project header, then adds the input options to the options description.
     * It then parses the command line arguments and stores them in a variables map.
     * If the "help" option is present, it prints the options and exits the program.
     * Finally, it calls the initialize method to print the compile configurations and precision information, and parse the command line arguments.
     */
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

    /**
     * @fn boost::program_options::variables_map &CommandLine::getArguments()
     * @brief Returns the parsed command line arguments.
     *
     * This method returns a reference to the variables map that contains the parsed command line arguments.
     */
    boost::program_options::variables_map &getArguments() {
        return variablesMap;
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

private:
    boost::program_options::options_description optionsDescription;
    boost::program_options::variables_map variablesMap;

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
                ("precision,p", boost::program_options::value<int>()->default_value(max_precision), "= Number of digits to represent long double")
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

    /**
     * @fn void CommandLine::initialize(boost::program_options::options_description &options)
     * @brief Initializes the class.
     *
     * This method initializes the class by printing the compile configurations and precision information, and parsing
     * the command line arguments.
     * If the "quiet" option is present, it returns without printing anything.
     */
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

    /**
     * @fn void CommandLine::printCompileConfigs()
     * @brief Prints the compile configurations to the console.
     *
     * This method prints the compiler ID, compiler version, compiler flags, and Boost version and libraries to the console.
     */
    static void printCompileConfigs() {
        std::cout<<"Build Configuration\n";
        std::cout<<"compiler: "<<CXX_COMPILER_ID<<" "<<CXX_COMPILER_VERSION<<"\n";
        std::cout<<"flags: "<<CXX_FLAGS<<"\n";
        std::cout<<"Boost: "<<Boost_VERSION<<Boost_LIBRARIES<<"\n";
        printLine();
    }
};
