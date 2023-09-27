/**
 * @file Parser.h
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief This file contains the Parser class which is responsible for parsing command line arguments and validating
 * user inputs.
 */

#pragma once

#include "InputsOutputs.h"
#include "CommandLine.h"
#include "FileParser.h"
#include "Helpers.h"

class Parser : public CommandLine<InputMatrices> {

public:
    explicit Parser(const HeaderInfo &headerInfo, const CommandLineArgs &args) : CommandLine(headerInfo, args) {}

    explicit Parser() = default;

protected:
    /**
   * @brief This function builds the input options for the program.
   *
   * @return A boost::program_options::options_description object containing the description of the input options.
   */
    void buildInputArguments(boost::program_options::options_description &values) override {
        values.add_options()
                ("order,n", boost::program_options::value<long double>()->default_value(0), "= order of the square matrix (natural number)")
                ("input-json,i", boost::program_options::value<std::string>(), "= input JSON containing L, U, and b")
                ("output-json,o", boost::program_options::value<std::string>(), "= path for the output JSON");
    }

    /**
     * @brief This function prints the input values.
     *
     * @param vm A boost::program_options::variables_map object containing the input values to be printed.
     *
     */
    void printInputArguments(boost::program_options::variables_map &vm) override {
        // retrieve the inputs
        const auto n  = static_cast<size_t>(vm["order"].as<long double>());
        const auto inputFilepath = vm["input-json"].as<std::string>();
        const auto outputFilepath =  vm["output-json"].as<std::string>();
        // list the parameters
        CommandLine::printLine();
        std::cout << std::setw(44) << "Inputs\n";
        CommandLine::printLine();
        std::cout << "\tMatrix order,  n: " << n << "\n";
        std::cout << "\tInput JSON,    i: " << inputFilepath << "\n";
        std::cout << "\tOutput JSON,   o: " << outputFilepath << "\n";
        CommandLine::printLine();
    }

    /**
     * @brief This function performs checks on the input parameters and prompts the user to enter valid inputs if the
     * current inputs are invalid.
     *
     * @param map A boost::program_options::variables_map object containing the input values to be checked.
     */
    /**
     * @brief This function performs checks on the input parameters and prompts the user to enter valid inputs if the
     * current inputs are invalid.
     *
     * @param map A boost::program_options::variables_map object containing the input values to be checked.
     */
    void performInputArgumentsCheck(boost::program_options::variables_map &map) override {

        std::string input;

        // Check if input file path is provided
        if(!map.count("input-json") || map["input-json"].empty() || !doesFileExist(map["input-json"].as<std::string>())) {
            while(!map.count("input-json") || map["input-json"].empty() || !doesFileExist(map["input-json"].as<std::string>())) {
                std::cerr << "Error: No input JSON filepath provided.\n" << std::endl;
                std::cout << "Enter input file path (file extension is .json): ";
                std::cin >> input;
                try {
                    replace(map, "input-json", input);
                } catch (const std::exception &) {
                    continue;
                }
            }
        }

        // Check if output file path is provided and writable
        if(!map.count("output-json") || map["output-json"].empty() || !isFileWritable(map["output-json"].as<std::string>())) {
            while(!map.count("output-json") || map["output-json"].empty() || !isFileWritable(map["output-json"].as<std::string>())) {
                std::cerr << "Error: No output JSON filepath provided.\n" << std::endl;
                std::cout << "Enter output file path (file extension is .json): ";
                std::cin >> input;
                try {
                    replace(map, "output-json", input);
                } catch (const std::exception &) {
                    continue;
                }
            }
        }
    }

    /**
     * @brief Builds the output object with the provided values from the command line
     *
     * @param inputs Reference to the object to be populated
     * @param values Reference to the boost::program_options::variables_map containing the command line values
     */
    void buildInputs(InputMatrices &inputs, boost::program_options::variables_map &values) override {

        // first, read the input file into a json map
        nlohmann::json inputMap;
        readJSON(values["input-json"].as<std::string>(), inputMap);

        // read the constants
        std::vector<long double> constants = inputMap["constants"];
        inputs.constants = MyBLAS::Vector(constants);

        // read the upper matrix, add it to LU, and subtract identity matrix to remove double counting
        const auto lower = MyBLAS::Matrix(std::vector<std::vector<long double>>(inputMap["lower"]));
        const auto upper = MyBLAS::Matrix(std::vector<std::vector<long double>>(inputMap["upper"]));

        if (lower.getCols() != upper.getCols() || lower.getRows() != upper.getRows()) {
            std::cerr<<"Error: Input lower and upper triangular matrices do not have the same dimensions, aborting.\n";
            exit(-1);
        }

        if (lower.getCols() != upper.getRows()) {
            std::cerr<<"Error: Input matrices not square, aborting.\n";
            exit(-1);
        }

        if(lower.getRows() != inputs.constants.size()) {
            std::cerr<<"Error: Input constants vector not order n, aborting.\n";
            exit(-1);
        }

        // set input order n
        const auto orderFromInputMatrixDimensions = lower.getRows();
        if(values["order"].defaulted()) {
            std::cout<<"Reading matrix order (n) from input matrix dimensions: "<<orderFromInputMatrixDimensions<<"\n";
            inputs.n = orderFromInputMatrixDimensions;
        } else { // user provided some value for n, validate against input dimensions
            const auto orderFromUser = static_cast<size_t>(values["order"].as<long double>());
            inputs.n = orderFromUser > orderFromInputMatrixDimensions ? orderFromInputMatrixDimensions : orderFromUser;
            if (orderFromUser > orderFromInputMatrixDimensions) {
                std::cerr<<"Warning: Matrix order (n) is larger than input matrices, defaulting to lower value\n";
            }
        }

        const auto identity = MyBLAS::Matrix<long double>::eye(inputs.n);
        inputs.LU = lower + upper - identity;

        // print the matrices since we are in verbose mode.
        if(!values.count("quiet")) {
            const auto precision = getCurrentPrecision();
            printLine();
            std::cout << "Lower Triangular Matrix (L):\n";
            printLine();
            std::cout << std::setprecision (precision) << lower;
            printLine();
            std::cout << "Upper Triangular Matrix (U):\n";
            printLine();
            std::cout << std::setprecision (precision) << upper;
            printLine();
            std::cout << "Constants Vector (b):\n";
            printLine();
            std::cout << std::setprecision (precision) << inputs.constants;
            printLine();
            printLine();
            std::cout << "LU Matrix:\n";
            printLine();
            std::cout << std::setprecision (precision) << inputs.LU;
        }
    }

};
