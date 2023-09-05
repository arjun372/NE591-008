
#pragma once

#include <boost/program_options.hpp>
#include <utility>
#include "CommandLine.h"


template <typename InputType, typename OutputType> class Project {

public:
    explicit Project(CommandLineArgs args) {
        cmdArgs = args;
    }

    virtual ~Project() = default;


    void execute() {
        initialize();
        performInputArgumentsCheck(terminal.getArguments());
        printInputArguments(terminal.getArguments());
        buildInputs(inputs, terminal.getArguments());
        timedRun();
    }

private:
    CommandLineArgs cmdArgs{};
    CommandLine terminal;
    InputType inputs;
    OutputType outputs;

    void initialize() {
        boost::program_options::options_description arguments("Parameters");
        buildInputArguments(arguments);
        terminal = CommandLine(buildHeaderInfo(), arguments, cmdArgs);
        inputs = InputType();
        outputs = OutputType();
    }

    void timedRun() {
        run(outputs, inputs, terminal.getArguments());
    }

protected:

    // Handle
    virtual HeaderInfo buildHeaderInfo() = 0;

    // Handle input arguments
    virtual void buildInputArguments(boost::program_options::options_description &inputArguments) = 0;

    virtual void printInputArguments(boost::program_options::variables_map &values) = 0;
    virtual void performInputArgumentsCheck(boost::program_options::variables_map &values) = 0;

    // fill inputs object based on values
    virtual void buildInputs(InputType &ToFill, boost::program_options::variables_map &values) = 0;

    // fill outputs object based on input and values
    virtual void run(OutputType &output, InputType &input, boost::program_options::variables_map &values) = 0;
};
