/**
 * @file Parser.h
 * @author Arjun Earthperson
 * @date 10/20/2023
 * @brief This file contains the Parser class which is responsible for parsing command line arguments and validating
 * user inputs.
 */

#ifndef NE591_008_OUTLAB10_PARSER_H
#define NE591_008_OUTLAB10_PARSER_H

#include "math/blas/system/Circuit.h"

#include "InputsOutputs.h"
#include "CheckBounds.h"
#include "FileParser.h"
#include "Helpers.h"

/**
 * @class Parser
 * @brief This class is responsible for parsing command line arguments and validating user inputs.
 *
 * The Parser class extends the CommandLine class template with OutLab10Inputs as the template argument.
 * It provides methods to build input arguments, print input arguments, perform checks on input arguments,
 * and build inputs based on the parsed arguments.
 */
class Parser : public CommandLine<OutLab10Inputs> {

  public:
    /**
     * @brief Constructor that initializes the Parser object with header information and command line arguments.
     *
     * @param headerInfo A constant reference to a HeaderInfo object containing the header information.
     * @param args A constant reference to a CommandLineArgs object containing the command line arguments.
     */
    explicit Parser(const HeaderInfo &headerInfo, const CommandLineArgs &args) : CommandLine<OutLab10Inputs>(headerInfo, args) {}

    /**
     * @brief Default constructor for the Parser class.
     *
     * This constructor is explicitly defined to allow creating Parser objects without any initialization.
     */
    explicit Parser() = default;

  protected:
    /**
     * @brief This function builds the input options for the program.
     *
     * @return A boost::program_options::options_description object containing the description of the input options.
     */
    void buildInputArguments(boost::program_options::options_description &values) override {

        boost::program_options::options_description solverOptions("Solver Options");
        solverOptions.add_options()
            ("threshold,t", boost::program_options::value<long double>(), "= convergence threshold [𝜀 > 0]")(
                "max-iterations,k", boost::program_options::value<long double>(), "= maximum iterations [n ∈ ℕ]")(
                "order,n", boost::program_options::value<long double>(), "= order of the square matrix [n ∈ ℕ]");

        boost::program_options::options_description fileOptions("File I/O Options");
        fileOptions.add_options()(
            "input-json,i", boost::program_options::value<std::string>(), "= input JSON containing A, and b")(
            "generate,g", "= Generate A,b ignoring input-json")(
            "output-json,o", boost::program_options::value<std::string>()->default_value("/dev/stdout"), "= path for the output JSON");

        values.add(solverOptions);
        values.add(fileOptions);
    }

    /**
     * @brief This function prints the input values.
     *
     * @param vm A boost::program_options::variables_map object containing the input values to be printed.
     *
     */
    void printInputArguments(boost::program_options::variables_map &vm) override {
        // list the parameters
        CommandLine::printLine();
        std::cout << std::setw(44) << "Inputs\n";
        CommandLine::printLine();
        const bool gen = vm.count("generate");
        const auto inputJson = vm.count("input-json") ? vm["input-json"].as<std::string>() : "None provided";
        std::cout << "\tGenerate A,b,            g: " << (gen ? "Yes" : "No") << "\n";
        std::cout << "\tInput JSON (for A, b),   i: " << (gen ? "[IGNORED] " : " ") << inputJson << "\n";
        std::cout << "\tOutput JSON (for x),     o: " << vm["output-json"].as<std::string>() << "\n";
        std::cout << "\tConvergence Threshold,   𝜀: " << vm["threshold"].as<long double>() << "\n";
        std::cout << "\tMax iterations,          k: " << static_cast<size_t>(vm["max-iterations"].as<long double>())
                  << "\n";
        std::cout << "\tMatrix order,            n: "
                  << (vm.count("order") ? std::to_string(static_cast<size_t>(vm["order"].as<long double>()))
                                        : "None provided, will be inferred from input JSON")<<std::endl;
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

        // only if not generating already
        if (!map.count("generate")) {
            // Check if input file path is provided
            if (!map.count("input-json") || map["input-json"].empty() ||
                !doesFileExist(map["input-json"].as<std::string>())) {
                while (!map.count("input-json") || map["input-json"].empty() ||
                       !doesFileExist(map["input-json"].as<std::string>())) {
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
        }

        // Check if output file path is provided and writable
        if (!map.count("output-json") || map["output-json"].empty() ||
            !isFileWritable(map["output-json"].as<std::string>())) {
            while (!map.count("output-json") || map["output-json"].empty() ||
                   !isFileWritable(map["output-json"].as<std::string>())) {
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

        // read the input json and populate the variables_map
        nlohmann::json inputMap;
        try {
            readJSON(map["input-json"].as<std::string>(), inputMap);
        } catch (...) {
            // initialize input map if no file was read
        }

        std::vector<std::function<bool(long double)>> checks;

        // add checks for parameters 𝜀 and k
        checks.clear();
        checks.emplace_back([](long double value) { return failsPositiveNumberCheck(value); });
        performChecksAndUpdateInput<long double>("threshold", inputMap, map, checks);

        checks.emplace_back([](long double value) { return failsWholeNumberCheck(value); });
        performChecksAndUpdateInput<long double>("max-iterations", inputMap, map, checks);

        if (map.count("order")) {
            performChecksAndUpdateInput<long double>("order", inputMap, map, checks);
        } else if (map.count("generate")) {
            checks.clear();
            checks.emplace_back([](long double value) { return failsNaturalNumberCheck(value); });
            performChecksAndUpdateInput<long double>("order", inputMap, map, checks);
        }
    }

    /**
     * @brief Builds the inputs
     */
    void buildInputs(OutLab10Inputs &input, boost::program_options::variables_map &values) override {

        // first, read the input file into a json map
        nlohmann::json inputMap;
        if (values.count("input-json")) {
            readJSON(values["input-json"].as<std::string>(), inputMap);
        }
        const bool generate = values.count("generate");
        if (generate) {
            if (!values.count("order")) {
                std::cerr << "Error: User did not provide matrix order n, aborting\n";
                exit(-1);
            }
            auto n = static_cast<size_t>(values["order"].as<long double>());
            input.input.n = n;
            MyBLAS::System::Circuit(n, input.input.coefficients, input.input.constants, input.known_solution);
        } else {
            // read the constants
            input.input.constants = MyBLAS::Vector(MyBLAS::Vector(std::vector<long double>(inputMap["constants"])));
            // read the coefficient matrix
            input.input.coefficients = MyBLAS::Matrix(std::vector<std::vector<long double>>(inputMap["coefficients"]));
        }

        if (!MyBLAS::isSquareMatrix(input.input.coefficients)) {
            std::cerr << "Error: Input coefficients matrix A not square, aborting.\n";
            exit(-1);
        }

        if (input.input.coefficients.getRows() != input.input.constants.size()) {
            std::cerr << "Error: Input constants vector not order n, aborting.\n";
            exit(-1);
        }

        if (!generate) {
            // set input order n
            const auto orderFromInputMatrixDimensions = input.input.coefficients.getRows();
            if (!values.count("order")) {
                std::cout << "Reading matrix order (n) from input matrix dimensions: " << orderFromInputMatrixDimensions
                          << "\n";
                input.input.n = orderFromInputMatrixDimensions;
            } else { // user provided some value for n, validate against input dimensions
                const auto orderFromUser = static_cast<size_t>(values["order"].as<long double>());
                input.input.n =
                    orderFromUser > orderFromInputMatrixDimensions ? orderFromInputMatrixDimensions : orderFromUser;
                if (orderFromUser > orderFromInputMatrixDimensions) {
                    std::cerr << "Warning: Matrix order (n) is larger than input matrix, defaulting to lower value\n";
                }
            }
        }

        input.input.threshold = values["threshold"].as<long double>();
        input.input.max_iterations = static_cast<size_t>(values["max-iterations"].as<long double>());

        if (!MyBLAS::isSquareMatrix(input.input.coefficients)) {
            std::cerr << "Warning: Input coefficients matrix A not square.\n";
        } else if (!values.count("quiet")) {
            std::cout << "Input coefficients matrix A is square.\n";
        }

        if (!MyBLAS::isSymmetricMatrix(input.input.coefficients)) {
            std::cerr << "Warning: Input coefficients matrix A is not symmetric.\n";
        } else if (!values.count("quiet")) {
            std::cout << "Input coefficients matrix A is symmetric.\n";
        }

        if (!MyBLAS::isDiagonallyDominant(input.input.coefficients)) {
            std::cerr << "Warning: Input coefficients matrix A is not diagonally dominant.\n";
        } else if (!values.count("quiet")) {
            std::cout << "Input coefficients matrix A is diagonally dominant.\n";
        }

        if (!MyBLAS::isPositiveDefiniteMatrix(input.input.coefficients)) {
            std::cerr << "Warning: Input coefficients matrix A is not positive definite.\n";
        } else if (!values.count("quiet")) {
            std::cout << "Input coefficients matrix A is positive definite.\n";
        }

        if (input.input.coefficients.getRows() != input.input.constants.size()) {
            std::cerr << "Error: Input constants vector 'b' not order n, aborting.\n";
            exit(-1);
        }

        input.input.n = input.input.coefficients.getRows();
    }
};

#endif // NE591_008_OUTLAB10_PARSER_H
