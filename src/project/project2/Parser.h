/**
 * @file Parser.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the Parser class which is responsible for parsing command line arguments and validating
 * user inputs.
 */

#ifndef NE591_008_PROJECT2_PARSER_H
#define NE591_008_PROJECT2_PARSER_H

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
            ",a", boost::program_options::value<MyBLAS::NumericType>(),"= Length of 1st dimension (+ve real)")(
            ",b", boost::program_options::value<MyBLAS::NumericType>(),"= Length of 2nd dimension (+ve real)")(
            ",m", boost::program_options::value<MyBLAS::NumericType>(), "= Number of mesh-points in 1st dimension")(
            ",n", boost::program_options::value<MyBLAS::NumericType>(), "= Number of mesh-points in 2nd dimension")(
            ",D", boost::program_options::value<MyBLAS::NumericType>(), "= Diffusion coefficient D (+ve real)")(
            "cross-section", boost::program_options::value<MyBLAS::NumericType>(), "= Removal cross-section Σₐ (+ve real)");

        boost::program_options::options_description files("Inputs/Outputs");
        files.add_options()(
            "input-parameter-json,i", boost::program_options::value<std::string>(), "= Path to input parameter JSON")(
            "source-terms-csv,s", boost::program_options::value<std::string>(), "= Path to source-terms 𝑞(𝑖,𝑗) CSV")(
            "output-results-json,o", boost::program_options::value<std::string>(), "= Path to output results JSON")(
            "flux-output-dir,f", boost::program_options::value<std::string>(), "= Path to computed flux 𝜙(𝑖,𝑗) CSV");

        boost::program_options::options_description methods("Solver Options");
        methods.add_options()
            ("use-LUP", "= Use LUP factorization")
            ("use-point-jacobi","= Use the Point-Jacobi method")
            ("use-SORJ", "= Use the SOR Jacobi method")
            ("use-gauss-seidel", "= Use the Gauss-Seidel method")
            ("use-SOR", "= Use the SOR method")
            ("use-SSOR", "= Use the symmetric SOR method")(
            "threshold,t", boost::program_options::value<MyBLAS::NumericType>(),"= convergence threshold [𝜀 > 0]")(
            "max-iterations,k", boost::program_options::value<MyBLAS::NumericType>(), "= maximum iterations [n ∈ ℕ]")(
            "relaxation-factor,w", boost::program_options::value<MyBLAS::NumericType>(), "= SOR weight, typical ω ∈ [0,2]");
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

        std::cout << std::setprecision(default_precision);
        // list the parameters
        CommandLine::printLine();
        std::cout << std::setw(44) << "Inputs\n";
        CommandLine::printLine();
        std::cout << "\tInput JSON,     i: " << (vm.count("input-parameter-json") ? vm["input-parameter-json"].as<std::string>() : "None") << "\n";
        std::cout << "\tOutput JSON,    o: " << (vm.count("output-results-json") ? vm["output-results-json"].as<std::string>() : "None") << "\n";
        std::cout << "\t----\n";
        std::cout << "\tConvergence Threshold,                  𝜀: " << vm["threshold"].as<MyBLAS::NumericType>() << "\n";
        std::cout << "\tMax iterations,                         k: " << static_cast<size_t>(vm["max-iterations"].as<MyBLAS::NumericType>())<< "\n";
        std::cout << "\tSOR weight,                             ω: "<< (vm.count("relaxation-factor") ? std::to_string(vm["relaxation-factor"].as<MyBLAS::NumericType>()) : "N/A") << "\n";
        std::cout << "\tUse LUP factorization                    : " << (vm["use-LUP"].as<bool>() ? "Yes" : "No") << "\n";
        std::cout << "\tUse Gauss-Seidel                         : " << (vm["use-gauss-seidel"].as<bool>() ? "Yes" : "No") << "\n";
        std::cout << "\tUse Point-Jacobi                         : " << (vm["use-point-jacobi"].as<bool>() ? "Yes" : "No") << "\n";
        std::cout << "\tUse SOR                                  : " << (vm["use-SOR"].as<bool>() ? "Yes" : "No") << "\n";
        std::cout << "\tUse Point-Jacobi with SOR                : " << (vm["use-SORJ"].as<bool>() ? "Yes" : "No") << "\n";
        std::cout << "\tUse symmetric SOR                        : " << (vm["use-SSOR"].as<bool>() ? "Yes" : "No") << "\n";
        std::cout << "\t----\n";
        std::cout << "\t1st dimension length,                   a: " << vm["a"].as<MyBLAS::NumericType>() << "\n";
        std::cout << "\t2nd dimension length,                   b: " << vm["b"].as<MyBLAS::NumericType>() << "\n";
        std::cout << "\t1st dimension mesh-points,              m: " << static_cast<size_t>(vm["m"].as<MyBLAS::NumericType>()) << "\n";
        std::cout << "\t2nd dimension mesh-points,              n: " << static_cast<size_t>(vm["n"].as<MyBLAS::NumericType>()) << "\n";
        std::cout << "\tDiffusion coefficient,                  D: " << (vm["D"].as<MyBLAS::NumericType>()) << "\n";
        std::cout << "\tMacroscopic removal cross-section,     Σₐ: " << (vm["cross-section"].as<MyBLAS::NumericType>()) << "\n";
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
        if (!map.count("flux-output-dir") && !quietMode) {
            std::cout << "WARNING: Flux matrix CSV folder path not provided. Writing results to console only.\n";
        }
        // else:
        // if filepath for output json is provided, it will be validated.
        // If the path is invalid, a new path will be accepted interactively
        else if (map["flux-output-dir"].empty() || !isFileWritable(map["flux-output-dir"].as<std::string>())) {
            while (!map.count("flux-output-dir") || map["flux-output-dir"].empty() ||
                   !isDirectoryWritable(map["flux-output-dir"].as<std::string>())) {
                std::cerr << "Error: No output folder provided.\n" << std::endl;
                std::cout << "Enter a folder name to store the computed fluxes: ";
                std::cin >> input;
                try {
                    replace(map, "flux-output-dir", input);
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
        performChecksAndUpdateInput<MyBLAS::NumericType>("a", inputMap["dimensions"], map, checks);
        performChecksAndUpdateInput<MyBLAS::NumericType>("b", inputMap["dimensions"], map, checks);

        // add checks for parameters m & n
        checks.clear();
        checks.emplace_back([](MyBLAS::NumericType value) { return failsNaturalNumberCheck(value); });
        performChecksAndUpdateInput<MyBLAS::NumericType>("m", inputMap["mesh"], map, checks);
        performChecksAndUpdateInput<MyBLAS::NumericType>("n", inputMap["mesh"], map, checks);

        // add checks for parameters D and Σₐ
        checks.clear();
        checks.emplace_back([](MyBLAS::NumericType value) { return failsNonNegativeNumberCheck(value); });
        performChecksAndUpdateInput<MyBLAS::NumericType>("D", inputMap, map, checks);
        performChecksAndUpdateInput<MyBLAS::NumericType>("cross-section", inputMap, map, checks);

        // add checks for parameters 𝜀 and k
        checks.clear();
        checks.emplace_back([](MyBLAS::NumericType value) { return failsPositiveNumberCheck(value); });
        performChecksAndUpdateInput<MyBLAS::NumericType>("threshold", inputMap, map, checks);
        checks.emplace_back([](MyBLAS::NumericType value) { return failsWholeNumberCheck(value); });
        performChecksAndUpdateInput<MyBLAS::NumericType>("max-iterations", inputMap, map, checks);

        auto methods = std::vector<std::string>(inputMap["methods"]);
        if(contains(methods, "LUP")) {
            replace(map, "use-LUP", asYesOrNo("yes"));
        } else {
            promptAndSetFlags("use-LUP", "LUP factorization method", map);
        }

        if(contains(methods, "point-jacobi")) {
            replace(map, "use-point-jacobi", asYesOrNo("yes"));
        } else {
            promptAndSetFlags("use-point-jacobi", "Point Jacobi method", map);
        }

        if(contains(methods, "SORJ")) {
            replace(map, "use-SORJ", asYesOrNo("yes"));
        } else {
            promptAndSetFlags("use-SORJ", "SOR Jacobi method", map);
        }

        if(contains(methods, "gauss-seidel")) {
            replace(map, "use-gauss-seidel", asYesOrNo("yes"));
        } else {
            promptAndSetFlags("use-gauss-seidel", "Gauss-Seidel method", map);
        }

        if(contains(methods, "SOR")) {
            replace(map, "use-SOR", asYesOrNo("yes"));
        } else {
            promptAndSetFlags("use-SOR", "SOR method", map);
        }

        if(contains(methods, "SSOR")) {
            replace(map, "use-SSOR", asYesOrNo("yes"));
        } else {
            promptAndSetFlags("use-SSOR", "symmetric SOR method", map);
        }

        if (map["use-SOR"].as<bool>() || map["use-SORJ"].as<bool>() || map["use-SSOR"].as<bool>()) {
            checks.clear();
            performChecksAndUpdateInput<MyBLAS::NumericType>("relaxation-factor", inputMap, map, checks);
        }
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

        inputs.diffusionParams
            .setA(map["a"].as<MyBLAS::NumericType>())
            .setB(map["b"].as<MyBLAS::NumericType>())
            .setM(static_cast<size_t>(map["m"].as<MyBLAS::NumericType>()))
            .setN(static_cast<size_t>(map["n"].as<MyBLAS::NumericType>()))
            .setDiffusionCoefficient(map["D"].as<MyBLAS::NumericType>())
            .setMacroscopicRemovalCrossSection(map["cross-section"].as<MyBLAS::NumericType>());

        const auto sourceTermsFilepath = map["source-terms-csv"].as<std::string>();
        readCSVRowWiseNoHeaders<MyBLAS::NumericType>(sourceTermsFilepath, inputs.sources);

        inputs.fluxOutputDirectory = map["flux-output-dir"].as<std::string>();

        if (inputs.sources.getRows() < 1 || inputs.sources.getCols() < 1) {
            std::cerr << "ERROR: No Source terms matrix dimension (rows=" << inputs.sources.getRows()
                      << ", columns=" << inputs.sources.getCols() << ")  can be less than 1!\n";
            std::cerr << "ABORTING.\n";
            exit(-1);
        }

        // 1st dimension internal nodes
        if (inputs.diffusionParams.getM() != inputs.sources.getRows()) {
            inputs.diffusionParams.setM(inputs.sources.getRows());
            std::cerr << "WARNING: Source terms matrix rows != m, overriding m to " << inputs.diffusionParams.getM() << std::endl;
        }

        // 2nd dimension internal nodes
        if (inputs.diffusionParams.getN() != inputs.sources.getCols()) {
            inputs.diffusionParams.setN(inputs.sources.getCols());
            std::cerr << "WARNING: Source terms matrix columns != n, overriding n to " << inputs.diffusionParams.getN() << std::endl;
        }

        inputs.solverParams.threshold = map["threshold"].as<MyBLAS::NumericType>();
        inputs.solverParams.max_iterations = static_cast<size_t>(map["max-iterations"].as<MyBLAS::NumericType>());
        inputs.solverParams.n = inputs.diffusionParams.getM() * inputs.diffusionParams.getN();

        if (map["use-LUP"].as<bool>()) {
            inputs.methods.insert(MyFactorizationMethod::Type::METHOD_LUP);
        }

        if (map["use-point-jacobi"].as<bool>()) {
            inputs.methods.insert(MyRelaxationMethod::Type::METHOD_POINT_JACOBI);
        }

        if (map["use-gauss-seidel"].as<bool>()) {
            inputs.methods.insert(MyRelaxationMethod::Type::METHOD_GAUSS_SEIDEL);
        }

        if (map["use-SOR"].as<bool>()) {
            inputs.methods.insert(MyRelaxationMethod::Type::METHOD_SOR);
        }

        if (map["use-SORJ"].as<bool>()) {
            inputs.methods.insert(MyRelaxationMethod::Type::METHOD_SORJ);
        }

        if (map["use-SSOR"].as<bool>()) {
            inputs.methods.insert(MyRelaxationMethod::Type::METHOD_SSOR);
        }

        if (map.count("relaxation-factor")) {
            inputs.solverParams.relaxation_factor = map["relaxation-factor"].as<MyBLAS::NumericType>();
        }

        if (!map.count("quiet")) {
            // list the parameters
            CommandLine::printLine();
            std::cout << std::setw(44) << "Intermediates\n";
            CommandLine::printLine();
            std::cout << "\tMesh spacing in the 1st dimension,     𝛿: " << inputs.diffusionParams.getDelta() << "\n";
            std::cout << "\tMesh spacing in the 2nd dimension,     𝛾: " << inputs.diffusionParams.getGamma() << "\n";
            CommandLine::printLine();
            std::cout << "Source terms 𝑞(𝑖,𝑗):\n";
            CommandLine::printLine();
            std::cout << inputs.sources;
            CommandLine::printLine();
        }
    }
};

#endif // NE591_008_PROJECT2_PARSER_H
