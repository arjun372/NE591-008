/**
 * @file Project.h
 * @author Arjun Earthperson
 * @date 08/30/2023
 * @brief This file contains the definition of the Project class template.
 *        The Project class is a generic class that represents a project that
 * can be executed.
 */

#ifndef NE591_008_PROJECT_H
#define NE591_008_PROJECT_H

#include "CommandLine.h"
#include <boost/program_options.hpp>
#include <utility>

/**
 * @class Project
 * @brief A generic class that represents a project that can be executed.
 * @tparam InputType The type of the input data.
 * @tparam CommandLineParserType The type of the command line parser.
 * @tparam OutputType The type of the output data.
 */
template <typename InputType, typename CommandLineParserType,
          typename OutputType>
class Project {

  public:
    /**
     * @brief A static assertion to ensure that CommandLineParserType is a
     * derived class of CommandLine.
     */
    static_assert(
        std::is_base_of<CommandLine<InputType>, CommandLineParserType>::value,
        "CommandLineParserType must be a derived class of CommandLine");

    /**
     * @brief Constructor that initializes the command line arguments.
     * @param args The command line arguments.
     */
    explicit Project(CommandLineArgs args) { cmdArgs = args; }

    /**
     * @brief Default virtual destructor.
     */
    virtual ~Project() = default;

    /**
     * @brief Executes the project.
     */
    void execute() {
        initialize();
        timedRun();
    }

    /**
     * @brief Returns the terminal.
     * @return The terminal.
     */
    CommandLineParserType getTerminal() const { return terminal; }

  private:
    CommandLineArgs cmdArgs{};
    CommandLineParserType terminal;
    OutputType outputs;

    /**
     * @brief Initializes the project.
     */
    void initialize() {
        terminal = CommandLineParserType(buildHeaderInfo(), cmdArgs);
        terminal.getInputs();
        outputs = OutputType();
    }

    /**
     * @brief Executes the project and measures the execution time.
     */
    void timedRun() {
        run(outputs, terminal.getInputs(), terminal.getArguments());
    }

  protected:
    /**
     * @brief Builds the header information.
     * @return The header information.
     */
    virtual HeaderInfo buildHeaderInfo() = 0;

    /**
     * @brief A hook for performing actions before the run.
     * @param output The output data.
     * @param input The input data.
     * @param values The variable map.
     */
    virtual void preRun(OutputType &output, InputType &input,
                        boost::program_options::variables_map &values) {}

    /**
     * @brief Executes the project.
     * @param output The output data.
     * @param input The input data.
     * @param values The variable map.
     */
    virtual void run(OutputType &output, InputType &input,
                     boost::program_options::variables_map &values) = 0;

    /**
     * @brief A hook for performing actions after the run.
     * @param output The output data.
     * @param input The input data.
     * @param values The variable map.
     */
    virtual void postRun(OutputType &output, InputType &input,
                         boost::program_options::variables_map &values) {}
};

#endif // NE591_008_PROJECT_H
