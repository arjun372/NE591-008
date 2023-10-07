
#pragma once

typedef struct Input {
    size_t n{};
    size_t m{};
    std::vector<long double> xData;
    std::vector<long double> fxData;
} LagrangePolynomialInputs;

class Parser : public CommandLine<LagrangePolynomialInputs> {

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
        values.add_options()("num-points,n", boost::program_options::value<long double>(),
                             "= (optional) number of interpolation points n")(
            "num-samples,m", boost::program_options::value<long double>(),
            "= number of Lagrange interpolation evaluation points")(
            "x-points,x",
            boost::program_options::value<std::vector<long double>>()->multitoken()->default_value(
                std::vector<long double>(), ""),
            "= distinct and sorted (x) interpolation points if --input-csv is unset")(
            "fx-points,f",
            boost::program_options::value<std::vector<long double>>()->multitoken()->default_value(
                std::vector<long double>(), ""),
            "= f(x=n) points if --use-fx-function and --input-csv are unset")(
            "input-csv,i", boost::program_options::value<std::string>(),
            "= path for input CSV file with two columns [x, f(x)]")(
            "output-csv,o", boost::program_options::value<std::string>(),
            "= path for output CSV file with five columns [i, x, f(x), L(x), E(x)]")("use-fx-function,F",
                                                                                     "= use bundled f(x=n) function");
    }

    /**
     * @brief This function prints the input values.
     *
     * @param vm A boost::program_options::variables_map object containing the input values to be printed.
     *
     */
    void printInputArguments(boost::program_options::variables_map &vm) override {

        // retrieve the inputs
        const auto precision = vm["precision"].as<int>();
        const auto n = static_cast<size_t>(vm["num-points"].as<long double>());
        const auto m = static_cast<size_t>(vm["num-samples"].as<long double>());
        const auto x = vm["x-points"].as<std::vector<long double>>();
        const auto fx = vm["fx-points"].as<std::vector<long double>>();

        // list the parameters
        CommandLine::printLine();
        std::cout << std::setw(44) << "Inputs\n";
        CommandLine::printLine();
        std::cout << std::setw(16) << "x" << std::setw(34) << "f(x)\n" << std::scientific;
        for (size_t i = 0; i < x.size(); i++) {
            const auto x_sign = (x[i] >= 0) ? "+" : "";
            const auto fx_sign = (fx[i] >= 0) ? "+" : "";
            std::cout << "\t" << x_sign << std::setprecision(precision) << x[i];
            std::cout << "\t\t\t" << fx_sign << std::setprecision(precision) << fx[i] << std::endl;
        }
        CommandLine::printLine();
        std::cout << "\tnum-points,     n: " << n << "\n";
        std::cout << "\tnum-samples,    m: " << m << "\n";
        if (vm.count("input-csv") && !vm["input-csv"].empty()) {
            std::cout << "\tinput-csv,      i: " << vm["input-csv"].as<std::string>() << "\n";
        }
        std::cout << "\toutput-csv,     o: " << vm["output-csv"].as<std::string>() << "\n";
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

        // Check if output CSV file path is provided and writable
        if (!map.count("output-csv") || map["output-csv"].empty() ||
            !isFileWritable(map["output-csv"].as<std::string>())) {
            while (!map.count("output-csv") || map["output-csv"].empty() ||
                   !isFileWritable(map["output-csv"].as<std::string>())) {
                std::cerr << "Error: No output CSV filepath provided." << std::endl;
                std::cout << "Enter output file path: ";
                std::cin >> input;
                try {
                    replace(map, "output-csv", input);
                } catch (const std::exception &) {
                    continue;
                }
            }
        }

        // If input-csv option is provided, read data from the file
        if (map.count("input-csv") && !map["input-csv"].empty()) {
            const std::string filename = map["input-csv"].as<std::string>();
            std::cout << "Reading from file: " << filename << std::endl;
            try {
                // Create a data map
                std::map<std::string, std::vector<long double>> dataMap;
                // Fill data, mapping csv headers (keys) to csv columns (value vectors)
                readCSV(filename, dataMap);

                // Read column x if provided and update the number of points
                if (dataMap.count("x")) {
                    replace(map, "x-points", dataMap["x"]);
                    replace(map, "num-points", static_cast<long double>(dataMap["x"].size()));
                }
                // Read column fx if provided
                if (dataMap.count("f(x)")) {
                    replace(map, "fx-points", dataMap["f(x)"]);
                }
            } catch (...) {
                std::cerr << "..failed. Aborting." << std::endl;
                exit(1);
            }
        }

        // Prompt the user to enter the number of interpolation points until a valid input is provided
        while (map["num-points"].empty() || failsNaturalNumberCheck(map["num-points"].as<long double>())) {
            std::cout << "Enter the number of interpolation points: ";
            std::cin >> input;
            try {
                replace(map, "num-points", asNumber(input));
            } catch (const std::exception &) {
                continue;
            }
        }

        // Prompt the user to enter the number of Lagrange interpolation evaluation points until a valid input is
        // provided
        while ((map["num-samples"].empty() || failsNaturalNumberCheck(map["num-samples"].as<long double>()))) {
            std::cout << "Enter the number of Lagrange interpolation evaluation points: ";
            std::cin >> input;
            try {
                replace(map, "num-samples", asNumber(input));
            } catch (const std::exception &) {
                continue;
            }
        }

        const auto n = static_cast<size_t>(map["num-points"].as<long double>());
        std::vector<long double> x_vec_inputs = map["x-points"].as<std::vector<long double>>();
        bool messageShown = false;
        // Prompt the user to enter the interpolation points until the required number of points is provided
        while (isUnfilledVector<long double>(map, "x-points", n)) {
            if (!messageShown) {
                std::cout << "Enter points for the interval x, sorted, and one at a time: " << std::endl;
                messageShown = true;
            }
            std::cin >> input;

            // attempting to insert non-increasing or non-unique value!
            if (!x_vec_inputs.empty() && asNumber(input) <= x_vec_inputs.back()) {
                std::cerr << "Cannot accept non-increasing values for x, try again.\n";
                continue;
            }

            try {
                x_vec_inputs.push_back(asNumber(input));
                replace(map, "x-points", x_vec_inputs);
            } catch (...) {
                std::cerr << "That wasn't a number, try again.\n";
                continue;
            }
        }

        // If the use-fx-function option is provided, fill the fx-points vector using the bundled f(x=n) function
        if (map.count("use-fx-function")) {
            const std::vector<long double> x = map["x-points"].as<std::vector<long double>>();
            try {
                std::vector<long double> fx_vec_inputs(x.size(), 0.0f);
                fill_fx(x, fx_vec_inputs);
                replace(map, "fx-points", fx_vec_inputs);
            } catch (...) {
                std::cerr << "Error: Executing external method 'fill_fx' failed. Aborting\n";
                exit(1);
            }
        }

        std::vector<long double> fx_vec_inputs;
        messageShown = false;
        // Prompt the user to enter the f(x) values until the required number of values is provided
        while (isUnfilledVector<long double>(map, "fx-points", n)) {
            if (!messageShown) {
                std::cout << "Enter " << n << " points for f(x) for every x, one at a time: " << std::endl;
                messageShown = true;
            }
            std::cin >> input;
            try {
                fx_vec_inputs.push_back(asNumber(input));
                replace(map, "fx-points", fx_vec_inputs);
            } catch (...) {
                std::cerr << "That wasn't a number, try again.\n";
                continue;
            }
        }
    }

    void buildInputs(Input &inputs, boost::program_options::variables_map &values) override {
        inputs.n = static_cast<size_t>(values["num-points"].as<long double>());
        inputs.m = static_cast<size_t>(values["num-samples"].as<long double>());
        inputs.xData = values["x-points"].as<std::vector<long double>>();
        inputs.fxData = values["fx-points"].as<std::vector<long double>>();
    }
};
