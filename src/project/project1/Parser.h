/**
 * @file Parser.h
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief This file contains the Parser class which is responsible for parsing command line arguments and validating
 * user inputs.
 */

#ifndef NE591_008_PROJECT1_PARSER_H
#define NE591_008_PROJECT1_PARSER_H

#include "CheckBounds.h"
#include "CommandLine.h"
#include "Compute.h"
#include "FileParser.h"
#include "Helpers.h"

class Parser : public CommandLine<SolverInputs> {

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
        values.add_options()(",a", boost::program_options::value<long double>(),
                             "= Length of 1st dimension (+ve real)")(",b", boost::program_options::value<long double>(),
                                                                     "= Length of 2nd dimension (+ve real)")(
            ",m", boost::program_options::value<long double>(), "= Number of mesh-points in 1st dimension")(
            ",n", boost::program_options::value<long double>(), "= Number of mesh-points in 2nd dimension")(
            ",D", boost::program_options::value<long double>(), "= Diffusion coefficient D (+ve real)")(
            "cross-section", boost::program_options::value<long double>(), "= Removal cross-section Σₐ (+ve real)")(
            "input-parameter-json,i", boost::program_options::value<std::string>(), "= Path to input parameter JSON")(
            "source-terms-csv,s", boost::program_options::value<std::string>(), "= Path to source-terms 𝑞(𝑖,𝑗) CSV")(
            "output-results-json,o", boost::program_options::value<std::string>(), "= Path to output results JSON")(
            "output-flux-csv,f", boost::program_options::value<std::string>(), "= Path to computed flux 𝜙(𝑖,𝑗) CSV");

        boost::program_options::options_description methods("Solver Methods");
        methods.add_options()("use-LUP", "= Use the LUP method")("use-point-jacobi",
                                                                 "= [DISABLED] Use the Point-Jacobi method")(
            "use-gauss-seidel", "= [DISABLED] Use the Gauss-Seidel method")("use-SOR",
                                                                            "= [DISABLED] Use the SOR method");
        values.add(methods);
    }

    /**
     * @brief This function prints the input values.
     *
     * @param vm A boost::program_options::variables_map object containing the input values to be printed.
     *
     */
    void printInputArguments(boost::program_options::variables_map &vm) override {

        if (vm.count("quiet")) {
            return;
        }

        std::cout << std::setprecision(default_precision);
        // list the parameters
        CommandLine::printLine();
        std::cout << std::setw(44) << "Inputs\n";
        CommandLine::printLine();
        std::cout << "\tInput JSON                              i: "
                  << (vm.count("input-parameter-json") ? vm["input-parameter-json"].as<std::string>() : "None") << "\n";
        std::cout << "\tOutput JSON                             o: "
                  << (vm.count("output-results-json") ? vm["output-results-json"].as<std::string>() : "None") << "\n";
        std::cout << "\t----\n";
        std::cout << "\tUse LUP method                           : " << (vm.count("use-LUP") ? "Yes" : "Yes") << "\n";
        std::cout << "\tUse Point-Jacobi method                  : " << (vm.count("use-point-jacobi") ? "Yes" : "No")
                  << "\n";
        std::cout << "\tUse Gauss-Seidel method                  : " << (vm.count("use-gauss-seidel") ? "Yes" : "No")
                  << "\n";
        std::cout << "\tUse SOR method                           : " << (vm.count("use-SOR") ? "Yes" : "No") << "\n";
        std::cout << "\t----\n";
        std::cout << "\t1st dimension length,                   a: " << vm["a"].as<long double>() << "\n";
        std::cout << "\t2nd dimension length,                   b: " << vm["b"].as<long double>() << "\n";
        std::cout << "\t1st dimension mesh-points,              m: " << static_cast<size_t>(vm["m"].as<long double>())
                  << "\n";
        std::cout << "\t2nd dimension mesh-points,              n: " << static_cast<size_t>(vm["n"].as<long double>())
                  << "\n";
        std::cout << "\tDiffusion coefficient,                  D: " << (vm["D"].as<long double>()) << "\n";
        std::cout << "\tMacroscopic removal cross-section,     Σₐ: " << (vm["cross-section"].as<long double>()) << "\n";
        CommandLine::printLine();
        std::cout << std::setprecision(vm["precision"].as<int>());
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

        const bool quietMode = map.count("quiet");
        std::string input;

        // FILE OPTIONAL
        // if a filepath for an input parameter json is not provided, parameters will be read from the terminal
        // interactively.
        if (!map.count("input-parameter-json") && !quietMode) {
            std::cout
                << "INFO: Input parameter JSON filepath not provided. Parameters will be specified interactively\n";
        }
        // else:
        // if a filepath of an input parameter json is provided, it will be validated.
        // If the path is invalid, a new path will be accepted interactively
        else if (map["input-parameter-json"].empty() || !doesFileExist(map["input-parameter-json"].as<std::string>())) {
            while (map["input-parameter-json"].empty() ||
                   !doesFileExist(map["input-parameter-json"].as<std::string>())) {
                std::cerr << "ERROR: Filepath for input parameter JSON is invalid.\n" << std::endl;
                std::cout << "Enter input parameter filepath (file extension is .json): ";
                std::cin >> input;
                try {
                    replace(map, "input-parameter-json", input);
                } catch (const std::exception &) {
                    continue;
                }
            }
        }

        // FILE REQUIRED
        // if a filepath of the source-terms CSV matrix is provided, it will be validated.
        // If the path is invalid, a new path will be accepted interactively
        if (!map.count("source-terms-csv") || map["source-terms-csv"].empty() ||
            !doesFileExist(map["source-terms-csv"].as<std::string>())) {
            std::cerr << "INFO: Source-terms matrix cannot be read interactively.\n" << std::endl;
            while (!map.count("source-terms-csv") || map["source-terms-csv"].empty() ||
                   !doesFileExist(map["source-terms-csv"].as<std::string>())) {
                std::cerr << "ERROR: Provided filepath for the source-terms matrix is invalid.\n" << std::endl;
                std::cout << "Enter a filepath for the source-terms matrix (file extension is .csv): ";
                std::cin >> input;
                try {
                    replace(map, "source-terms-csv", input);
                } catch (const std::exception &) {
                    continue;
                }
            }
        }

        // FILE OPTIONAL
        // if a filepath for output json is not provided, outputs will be written to the terminal only.
        if (!map.count("output-results-json") && !quietMode) {
            std::cout << "WARNING: Output results JSON filepath not provided. Writing results to console only.\n";
        }
        // else:
        // if filepath for output json is provided, it will be validated.
        // If the path is invalid, a new path will be accepted interactively
        else if (map["output-results-json"].empty() || !isFileWritable(map["output-results-json"].as<std::string>())) {
            while (!map.count("output-results-json") || map["output-results-json"].empty() ||
                   !isFileWritable(map["output-results-json"].as<std::string>())) {
                std::cerr << "Error: No output JSON filepath provided.\n" << std::endl;
                std::cout << "Enter output file path (file extension is .json): ";
                std::cin >> input;
                try {
                    replace(map, "output-results-json", input);
                } catch (const std::exception &) {
                    continue;
                }
            }
        }

        // FILE OPTIONAL
        // if a filepath for output json is not provided, outputs will be written to the terminal only.
        if (!map.count("output-flux-csv") && !quietMode) {
            std::cout << "WARNING: Compute flux matrix CSV filepath not provided. Writing results to console only.\n";
        }
        // else:
        // if filepath for output json is provided, it will be validated.
        // If the path is invalid, a new path will be accepted interactively
        else if (map["output-flux-csv"].empty() || !isFileWritable(map["output-flux-csv"].as<std::string>())) {
            while (!map.count("output-flux-csv") || map["output-flux-csv"].empty() ||
                   !isFileWritable(map["ooutput-flux-csv"].as<std::string>())) {
                std::cerr << "Error: No output CSV filepath provided.\n" << std::endl;
                std::cout << "Enter calculated flux matrix file path (file extension is .csv): ";
                std::cin >> input;
                try {
                    replace(map, "output-flux-csv", input);
                } catch (const std::exception &) {
                    continue;
                }
            }
        }

        // read the input json and populate the variables_map
        nlohmann::json inputMap;
        try {
            readJSON(map["input-parameter-json"].as<std::string>(), inputMap);
        } catch (...) {
            // initialize input map if no file was read
            inputMap["dimensions"] = {};
            inputMap["mesh"] = {};
        }

        std::vector<std::function<bool(long double)>> checks;

        // add checks for parameters a & b
        checks.clear();
        checks.emplace_back([](long double value) { return failsPositiveNumberCheck(value); });
        performChecksAndUpdateInput<long double>("a", inputMap["dimensions"], map, checks);
        performChecksAndUpdateInput<long double>("b", inputMap["dimensions"], map, checks);

        // add checks for parameters m & n
        checks.clear();
        checks.emplace_back([](long double value) { return failsNaturalNumberCheck(value); });
        performChecksAndUpdateInput<long double>("m", inputMap["mesh"], map, checks);
        performChecksAndUpdateInput<long double>("n", inputMap["mesh"], map, checks);

        // add checks for parameters D and Σₐ
        checks.clear();
        checks.emplace_back([](long double value) { return failsNonNegativeNumberCheck(value); });
        performChecksAndUpdateInput<long double>("D", inputMap, map, checks);
        performChecksAndUpdateInput<long double>("cross-section", inputMap, map, checks);
    }

    /**
     * @brief Builds the input matrices for the MyBLAS library from a JSON file.
     *
     * This function reads the input JSON file and extracts the coefficient matrix and constants vector.
     * It also validates the dimensions of the input matrices and sets the order of the system.
     * If the verbose mode is enabled, it prints the input matrices.
     *
     * @param inputs Reference to a MyBLAS::InputMatrices object to store the input matrices.
     * @param map Reference to a boost::program_options::variables_map object containing the command line arguments.
     */
    void buildInputs(SolverInputs &inputs, boost::program_options::variables_map &map) override {

        inputs.a = map["a"].as<long double>();
        inputs.b = map["b"].as<long double>();
        inputs.m = static_cast<size_t>(map["m"].as<long double>());
        inputs.n = static_cast<size_t>(map["n"].as<long double>());
        inputs.diffusion_coefficient = map["D"].as<long double>();
        inputs.macroscopic_removal_cross_section = map["cross-section"].as<long double>();
        calculate_mesh_spacings(inputs); // calculate delta and gamma

        const auto sourceTermsFilepath = map["source-terms-csv"].as<std::string>();
        readCSVRowWiseNoHeaders<long double>(sourceTermsFilepath, inputs.sources);

        if (inputs.sources.getRows() < 1 || inputs.sources.getCols() < 1) {
            std::cerr << "ERROR: No Source terms matrix dimension (rows=" << inputs.sources.getRows()
                      << ", columns=" << inputs.sources.getCols() << ")  can be less than 1!\n";
            std::cerr << "ABORTING.\n";
            exit(-1);
        }

        // 1st dimension internal nodes
        if (inputs.m != inputs.sources.getRows()) {
            inputs.m = inputs.sources.getRows();
            std::cerr << "WARNING: Source terms matrix rows != m, overriding m to " << inputs.m << std::endl;
        }

        // 2nd dimension internal nodes
        if (inputs.n != inputs.sources.getCols()) {
            inputs.n = inputs.sources.getCols();
            std::cerr << "WARNING: Source terms matrix columns != n, overriding n to " << inputs.n << std::endl;
        }

        if (!map.count("quiet")) {
            // list the parameters
            CommandLine::printLine();
            std::cout << std::setw(44) << "Intermediates\n";
            CommandLine::printLine();
            std::cout << "\tMesh spacing in the 1st dimension,     𝛿: " << inputs.delta << "\n";
            std::cout << "\tMesh spacing in the 2nd dimension,     𝛾: " << inputs.gamma << "\n";
            CommandLine::printLine();
            std::cout << "Source terms 𝑞(𝑖,𝑗):\n";
            CommandLine::printLine();
            std::cout << inputs.sources;
            CommandLine::printLine();
        }
    }
};

#endif // NE591_008_PROJECT1_PARSER_H
