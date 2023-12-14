/**
 * @file Parser.h
 * @author Arjun Earthperson
 * @date 10/20/2023
 * @brief This file contains the Parser class which is responsible for parsing command line arguments and validating
 * user inputs.
 */

#ifndef NE591_008_INLAB8_PARSER_H
#define NE591_008_INLAB8_PARSER_H

#include "math/blas/system/Circuit.h"

#include "CheckBounds.h"
#include "FileParser.h"
#include "Helpers.h"

/**
 * @class Parser
 * @brief This class is responsible for parsing command line arguments and validating user inputs.
 *
 * The Parser class extends the CommandLine class template with OutLab7Inputs as the template argument.
 * It provides methods to build input arguments, print input arguments, perform checks on input arguments,
 * and build inputs based on the parsed arguments.
 */
class Parser : public CommandLine<OutLab7Inputs> {

  public:
    /**
     * @brief Constructor that initializes the Parser object with header information and command line arguments.
     *
     * @param headerInfo A constant reference to a HeaderInfo object containing the header information.
     * @param args A constant reference to a CommandLineArgs object containing the command line arguments.
     */
    explicit Parser(const HeaderInfo &headerInfo, const CommandLineArgs &args) : CommandLine<OutLab7Inputs>(headerInfo, args) {}

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
            ("stopping-criterion,e", "= stopping criterion [+ve R]")
            ("max-iterations,k", "= maximum iterations [k ∈ ℕ]");

        boost::program_options::options_description fileOptions("File I/O Options");
        fileOptions.add_options()(
            "input-json,i", boost::program_options::value<std::string>(), "= input JSON containing n")(
            "output-json,o", boost::program_options::value<std::string>(), "= path for the output JSON");

//        "source-terms-csv,s", boost::program_options::value<std::string>, "= Path to source-terms 𝑞(𝑖,𝑗) CSV")(
//"output-results-json,o", boost::program_options::value<std::string>(), "= Path to output results JSON")(
//"output-flux-csv,f", boost::program_options::value<std::string>(), "= Path to computed flux 𝜙(𝑖,𝑗) CSV");

        values.add_options()
           ("slab-width,L", boost::program_options::value<long double>(), "= slab width [+ve R]")(
            "num-angles,N", boost::program_options::value<long double>(), "= number of angles [N ∈ ℕ]")(
            "num-cells,I", boost::program_options::value<long double>(), "= number of cells [I ∈ ℕ]")(
            "total-cross-section,T", boost::program_options::value<long double>(), "= macroscopic total cross-section [+ve R]")(
            "scattering-cross-section,S", boost::program_options::value<long double>(), "= scattering macroscopic cross-section [+ve R]")(
            "source-terms-csv,s", boost::program_options::value<std::string>(), "= path to source-terms 𝑞(𝑖,𝑗) CSV");

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
        const auto inputJson = vm.count("input-json") ? vm["input-json"].as<std::string>() : "None provided";
        std::cout << "\tInput JSON,              i: " << inputJson << "\n";
        std::cout << "\tOutput JSON,             o: " << vm["output-json"].as<std::string>() << "\n";
        std::cout << "\tSum Upper Limit,         n: " << std::to_string(static_cast<size_t>(vm["n"].as<long double>())) << "\n";
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

        // add checks for parameter n
        checks.clear();
        checks.emplace_back([](long double value) { return failsNaturalNumberCheck(value); });
        performChecksAndUpdateInput<long double>("n", inputMap, map, checks);
    }

    /**
     * @brief Builds the inputs
     */
    void buildInputs(OutLab7Inputs &input, boost::program_options::variables_map &values) override {

        // first, read the input file into a json map
        nlohmann::json inputMap;
        if (values.count("input-json")) {
            readJSON(values["input-json"].as<std::string>(), inputMap);
        }

        input.n = static_cast<size_t>(values["n"].as<long double>());
        input.outputJSON = values["output-json"].as<std::string>();
    }
};

#endif // NE591_008_INLAB8_PARSER_H
