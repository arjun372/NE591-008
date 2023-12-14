/**
 * @file Parser.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the Parser class which is responsible for parsing command line arguments and validating
 * user inputs.
 */

#ifndef NE591_008_OUTLAB13_PARSER_H
#define NE591_008_OUTLAB13_PARSER_H

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
        values.add_options()(
            ",L", boost::program_options::value<MyBLAS::NumericType>(),"= Length of 1st dimension (+ve real)")(
            ",rho", boost::program_options::value<MyBLAS::NumericType>(),"= removal term [linear]")(
            ",beta", boost::program_options::value<MyBLAS::NumericType>(), "= removal term [non-linear]")(
            ",n", boost::program_options::value<MyBLAS::NumericType>(), "= Number of nodes per dimension excluding boundary nodes");

        boost::program_options::options_description files("Inputs/Outputs");
        files.add_options()(
            "input-parameter-json,i", boost::program_options::value<std::string>(), "= Path to input parameter JSON")(
            "output-results-json,o", boost::program_options::value<std::string>()->default_value("/dev/stdout"), "= Path to output results JSON");

        boost::program_options::options_description methods("Solver Options");
        methods.add_options()
            ("use-fixed-point", "= Use fixed point iteration")
            ("use-newton","= Use newton's method")(
            "threshold,t", boost::program_options::value<MyBLAS::NumericType>(),"= convergence threshold [𝜀 > 0]")(
            "max-iterations,k", boost::program_options::value<MyBLAS::NumericType>(), "= maximum iterations [n ∈ ℕ]");
        values.add(methods);
        values.add(files);
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

        std::cout << std::setprecision(static_cast<int>(default_precision));
        // list the parameters
        CommandLine::printLine();
        std::cout << std::setw(44) << "Inputs\n";
        CommandLine::printLine();
        std::cout << "\tInput JSON,     i: " << (vm.count("input-parameter-json") ? vm["input-parameter-json"].as<std::string>() : "None") << "\n";
        std::cout << "\tOutput JSON,    o: " << (vm.count("output-results-json") ? vm["output-results-json"].as<std::string>() : "None") << "\n";
        std::cout << "\t----\n";
        std::cout << "\tConvergence Threshold,                  𝜀: " << vm["threshold"].as<MyBLAS::NumericType>() << "\n";
        std::cout << "\tMax iterations,                         k: " << static_cast<size_t>(vm["max-iterations"].as<MyBLAS::NumericType>())<< "\n";
        std::cout << "\tUse Fixed Point Iterations               : " << (vm["use-fixed-point"].as<bool>() ? "Yes" : "No") << "\n";
        std::cout << "\tUse Newton's method                      : " << (vm["use-newton"].as<bool>() ? "Yes" : "No") << "\n";
        std::cout << "\t----\n";
        std::cout << "\tLength,                                 L: " << vm["L"].as<MyBLAS::NumericType>() << "\n";
        std::cout << "\trho,                                  rho: " << vm["rho"].as<MyBLAS::NumericType>() << "\n";
        std::cout << "\tbeta,                                beta: " << vm["beta"].as<MyBLAS::NumericType>() << "\n";
        std::cout << "\tMesh-Points,                            n: " << static_cast<size_t>(vm["n"].as<MyBLAS::NumericType>()) << "\n";
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

        // read the input json and populate the variables_map
        nlohmann::json inputMap;
        try {
            readJSON(map["input-parameter-json"].as<std::string>(), inputMap);
        } catch (...) {
            // initialize input map if no file was read
            inputMap["dimensions"] = nlohmann::json::object();
            inputMap["mesh"] = nlohmann::json::object();
            inputMap["methods"] = nlohmann::json::array();
        }

        std::vector<std::function<bool(MyBLAS::NumericType)>> checks;

        // add checks for parameters a & b
        checks.clear();
        checks.emplace_back([](MyBLAS::NumericType value) { return failsPositiveNumberCheck(value); });
        performChecksAndUpdateInput<MyBLAS::NumericType>("L", inputMap["dimensions"], map, checks);

        // add checks for parameters m & n
        checks.clear();
        checks.emplace_back([](MyBLAS::NumericType value) { return failsNaturalNumberCheck(value); });
        performChecksAndUpdateInput<MyBLAS::NumericType>("n", inputMap["mesh"], map, checks);

        // add checks for parameters D and Σₐ
        checks.clear();
        performChecksAndUpdateInput<MyBLAS::NumericType>("rho", inputMap, map, checks);
        performChecksAndUpdateInput<MyBLAS::NumericType>("beta", inputMap, map, checks);

        // add checks for parameters 𝜀 and k
        checks.clear();
        checks.emplace_back([](MyBLAS::NumericType value) { return failsPositiveNumberCheck(value); });
        performChecksAndUpdateInput<MyBLAS::NumericType>("threshold", inputMap, map, checks);
        checks.emplace_back([](MyBLAS::NumericType value) { return failsWholeNumberCheck(value); });
        performChecksAndUpdateInput<MyBLAS::NumericType>("max-iterations", inputMap, map, checks);

        promptAndSetFlags("use-fixed-point", "fixed point iteration", map);

        promptAndSetFlags("use-newton", "newton iteration", map);

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
    void buildInputs(SolverInputs &input, boost::program_options::variables_map &map) override {

        input.L = map["L"].as<MyBLAS::NumericType>();
        input.rho = map["rho"].as<MyBLAS::NumericType>();
        input.beta = map["beta"].as<MyBLAS::NumericType>();
        input.n = static_cast<size_t>(map["n"].as<MyBLAS::NumericType>());

//        input.diffusionParams
//            .setA(map["L"].as<MyBLAS::NumericType>())
//            .setN(static_cast<size_t>(map["n"].as<MyBLAS::NumericType>()))
//            .setDiffusionCoefficient(map["D"].as<MyBLAS::NumericType>())
//            .setMacroscopicRemovalCrossSection(map["cross-section"].as<MyBLAS::NumericType>());
//
//        const auto sourceTermsFilepath = map["source-terms-csv"].as<std::string>();
//        readCSVRowWiseNoHeaders<MyBLAS::NumericType>(sourceTermsFilepath, input.sources);
//
//        input.fluxOutputDirectory = map["flux-output-dir"].as<std::string>();
//
//        if (input.sources.getRows() < 1 || input.sources.getCols() < 1) {
//            std::cerr << "ERROR: No Source terms matrix dimension (rows=" << input.sources.getRows()
//                      << ", columns=" << input.sources.getCols() << ")  can be less than 1!\n";
//            std::cerr << "ABORTING.\n";
//            exit(-1);
//        }
//
//        // 1st dimension internal nodes
//        if (input.diffusionParams.getM() != input.sources.getRows()) {
//            input.diffusionParams.setM(input.sources.getRows());
//            std::cerr << "WARNING: Source terms matrix rows != m, overriding m to " << input.diffusionParams.getM() << std::endl;
//        }
//
//        // 2nd dimension internal nodes
//        if (input.diffusionParams.getN() != input.sources.getCols()) {
//            input.diffusionParams.setN(input.sources.getCols());
//            std::cerr << "WARNING: Source terms matrix columns != n, overriding n to " << input.diffusionParams.getN() << std::endl;
//        }
//
        input.solverParams.convergence_threshold = map["threshold"].as<MyBLAS::NumericType>();
        input.solverParams.max_iterations = static_cast<size_t>(map["max-iterations"].as<MyBLAS::NumericType>());
        input.solverParams.n = input.diffusionParams.getM() * input.diffusionParams.getN();

        if (map["use-fixed-point"].as<bool>()) {
            input.methods.insert(MyRelaxationMethod::Type::METHOD_FIXED_POINT);
        }

        if (map["use-newton"].as<bool>()) {
            input.methods.insert(MyRelaxationMethod::Type::METHOD_NEWTON);
        }

//        if (map["use-SORPJ"].as<bool>()) {
//            input.methods.insert(MyRelaxationMethod::Type::METHOD_SORPJ);
//        }
//
//        if (map["use-gauss-seidel"].as<bool>()) {
//            input.methods.insert(MyRelaxationMethod::Type::METHOD_GAUSS_SEIDEL);
//        }
//
//        if (map["use-symmetric-gauss-seidel"].as<bool>()) {
//            input.methods.insert(MyRelaxationMethod::Type::METHOD_SYMMETRIC_GAUSS_SEIDEL);
//        }
//
//        if (map["use-SOR"].as<bool>()) {
//            input.methods.insert(MyRelaxationMethod::Type::METHOD_SOR);
//        }
//
//        if (map["use-SSOR"].as<bool>()) {
//            input.methods.insert(MyRelaxationMethod::Type::METHOD_SSOR);
//        }
//
//        if (map.count("relaxation-factor")) {
//            input.solverParams.relaxation_factor = map["relaxation-factor"].as<MyBLAS::NumericType>();
//        }
//
//        if (map.count("bench")) {
//            input.numRuns = map.count("bench-runs") ? static_cast<size_t>(map["bench-runs"].as<long double>()) : 1;
//            input.timeout = map.count("bench-timeout") ? map["bench-timeout"].as<long double>() : 0;
//        } else {
//            input.numRuns = 1;
//            input.timeout = 0;
//        }
//
//        if (!map.count("quiet")) {
//            // list the parameters
//            CommandLine::printLine();
//            std::cout << std::setw(44) << "Intermediates\n";
//            CommandLine::printLine();
//            std::cout << "\tMesh spacing in the 1st dimension,     𝛿: " << input.diffusionParams.getDelta() << "\n";
//            std::cout << "\tMesh spacing in the 2nd dimension,     𝛾: " << input.diffusionParams.getGamma() << "\n";
//            CommandLine::printLine();
//            std::cout << "Source terms 𝑞(𝑖,𝑗):\n";
//            CommandLine::printLine();
//            std::cout << input.sources;
//            CommandLine::printLine();
//        }
    }
};

#endif // NE591_008_OUTLAB13_PARSER_H
