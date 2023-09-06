
#pragma once

#include <boost/program_options.hpp>
#include <utility>
#include "CommandLine.h"


template <typename InputType, typename CommandLineParserType, typename OutputType> class Project {

public:

    // Use static_assert and std::is_base_of to enforce T being a derived class of MyBaseClass
    static_assert(std::is_base_of<CommandLine<InputType>, CommandLineParserType>::value, "CommandLineParserType must be a derived class of CommandLine");

    explicit Project(CommandLineArgs args) {
        cmdArgs = args;
    }

    virtual ~Project() = default;


    void execute() {
        initialize();
        timedRun();
    }

private:
    CommandLineArgs cmdArgs{};
    CommandLineParserType terminal;
    OutputType outputs;

    void initialize() {
        terminal = CommandLineParserType(buildHeaderInfo(), cmdArgs);
        terminal.getInputs();
        outputs = OutputType();
    }

    void timedRun() {
        run(outputs, terminal.getInputs(), terminal.getArguments());
    }

protected:

    // Handle
    virtual HeaderInfo buildHeaderInfo() = 0;

    virtual void preRun(OutputType &output, InputType &input, boost::program_options::variables_map &values) {}
    virtual void run(OutputType &output, InputType &input, boost::program_options::variables_map &values) = 0;
    virtual void postRun(OutputType &output, InputType &input, boost::program_options::variables_map &values) {}
};
