
#pragma once

enum IntegrationRule {
    RULE_NONE,
    RULE_COMPOSITE_TRAPEZOIDAL,
    RULE_COMPOSITE_SIMPSONS,
    RULE_GAUSSIAN_QUADRATURE,
};

const char* ruleKey(IntegrationRule value) {
    static const char* IntegrationRuleKeys[] = {
            "none",
            "trapezoidal",
            "simpsons",
            "gaussian"
    };
    return IntegrationRuleKeys[static_cast<int>(value)];
}


typedef struct Input {
    long double a = 0;
    long double b = 0;
    size_t m = -1;
    bool flip_integral = false;
    std::set<IntegrationRule> integral_types = { RULE_NONE };

    void toJSON(nlohmann::json &jsonMap) {
        jsonMap["a"] = a;
        jsonMap["b"] = b;
        jsonMap["m"] = m;
        jsonMap["rules"] = [this]() -> std::vector<std::string> {
            std::vector<std::string> result;
            std::transform(integral_types.begin(), integral_types.end(), std::back_inserter(result), [](IntegrationRule rule) {
                return ruleKey(rule);
            });
            return result;
        }();
    }
} NewtonCotesInputs;

typedef struct Output {
    long double h;
    long double integral;
    void toJSON(nlohmann::json &jsonMap) {
        jsonMap["h"] = h;
        jsonMap["integral"] = integral;
    }
} NewtonCotesOutputs;

typedef std::map<std::string, std::string> Dictionary;

class Parser : public CommandLine<NewtonCotesInputs> {

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
                ("start,a", boost::program_options::value<long double>(), "= Starting value a for the integration interval [a,b]")
                ("stop,b", boost::program_options::value<long double>(), "= Stopping value b for the integration interval [a,b]")
                ("num-intervals,m", boost::program_options::value<long double>(), "= Number of equal intervals from [a,b]")
                ("use-trapezoidal,t", boost::program_options::value<bool>()->default_value(true), "= Set quadrature rule to the composite trapezoidal method")
                ("use-simpsons,s", boost::program_options::value<bool>()->default_value(true), "= Set quadrature rule to the composite Simpson's method")
                ("use-gaussian-quadratures,g", boost::program_options::value<bool>()->default_value(true), "= Use Gaussian quadratures for as the quadrature rule")
                ("output-json,o", boost::program_options::value<std::string>(), "= path for the output JSON file");
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
        const auto a  = vm["start"].as<long double>();
        const auto b  = vm["stop"].as<long double>();
        const auto m  = static_cast<size_t>(vm["num-intervals"].as<long double>());
        const auto use_trapezoidal  = vm["use-trapezoidal"].as<bool>();
        const auto use_simpson  = vm["use-simpsons"].as<bool>();
        const auto use_gaussian  = vm["use-gaussian-quadratures"].as<bool>();

        // list the parameters
        CommandLine::printLine();
        std::cout << std::setw(44) << "Inputs\n";
        CommandLine::printLine();
        std::cout << "\tInterval Count,            m: " << m << "\n";
        std::cout << "\tInterval Start,            a: " << std::setprecision(precision) << a << "\n";
        std::cout << "\tInterval Stop,             b: " << std::setprecision(precision) << b << "\n";
        std::cout << "\tOutput JSON,               o: " << vm["output-json"].as<std::string>() << "\n";
        std::cout << "\tUse Trapezoidal Rule,      t: " << (use_trapezoidal ? "yes" : "no") << "\n";
        std::cout << "\tUse Simpson's Rule,        s: " << (use_simpson ? "yes" : "no") << "\n";
        std::cout << "\tUse Gaussian Quadratures,  g: " << (use_gaussian ? "yes" : "no") << "\n";
        CommandLine::printLine();
    }

    // flip the interval if a>b
    // if a==b, trivially 0
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

        // Prompt the user to enter the interval starting point a
        while (map["start"].empty()) {
            std::cout << "Enter the starting value (a) for the integration interval [a,b]: ";
            std::cin >> input;
            try {
                replace(map, "start", asNumber(input));
            } catch (const std::exception &) {
                continue;
            }
        }

        const auto a = map["start"].as<long double>();

        // Prompt the user to enter the interval end point b
        while (map["stop"].empty()) {
            std::cout << "Enter the stopping value (b) for the integration interval [a,b]: ";
            std::cin >> input;
            if (a == asNumber(input)) {
                std::cout << "Warning: Integral for f(x) over [a,a] is trivially 0.\n";
            }
            if (a > asNumber(input)) {
                std::cout << "Warning: Since (a > b), integral will be performed for -f(x) over [b, a]\n";
            }
            try {
                replace(map, "stop", asNumber(input));
            } catch (const std::exception &) {
                continue;
            }
        }

        while(map["num-intervals"].empty() || failsNaturalNumberCheck(map["num-intervals"].as<long double>())) {
            std::cout<<"Enter the number of integration intervals (m): ";
            std::cin >> input;
            try {
                replace(map, "num-intervals", asNumber(input));
            } catch (const std::exception &) {
                continue;
            }
        }

        bool trapezoidal_set = !map["use-trapezoidal"].defaulted();
        while(!trapezoidal_set) {
            try {
                std::cout<<"Would you like to use the trapezoidal rule? [YES/no]: ";
                std::cin >> input;
                replace(map, "use-trapezoidal", asYesOrNo(input));
                trapezoidal_set = true;
            } catch (const std::exception &) {
                continue;
            }
        }

        bool simpsons_set = !map["use-simpsons"].defaulted();
        while(!simpsons_set) {
            try {
                std::cout<<"Would you like to use Simpson's rule? [YES/no]: ";
                std::cin >> input;
                replace(map, "use-simpsons", asYesOrNo(input));
                simpsons_set = true;
            } catch (const std::exception &) {
                continue;
            }
        }

        bool gaussian_set = !map["use-gaussian-quadratures"].defaulted();
        while(!gaussian_set) {
            try {
                std::cout<<"Would you like to use Gaussian quadratures? [YES/no]: ";
                std::cin >> input;
                replace(map, "use-gaussian-quadratures", asYesOrNo(input));
                gaussian_set = true;
            } catch (const std::exception &) {
                continue;
            }
        }
    }

    //TODO:: document
    void buildInputs(NewtonCotesInputs &inputs, boost::program_options::variables_map &values) override {
        inputs.a = static_cast<long double>(values["start"].as<long double>());
        inputs.b = static_cast<long double>(values["stop"].as<long double>());
        inputs.flip_integral = inputs.a > inputs.b;
        // flip interval if a > b
        if(inputs.flip_integral) {
            auto temp = inputs.b;
            inputs.b = inputs.a;
            inputs.a = temp;
        }

        inputs.m = static_cast<size_t>(values["num-intervals"].as<long double>());

        inputs.integral_types = {};
        if(values["use-trapezoidal"].as<bool>()) {
            inputs.integral_types.insert(RULE_COMPOSITE_TRAPEZOIDAL);
        }

        if(values["use-simpsons"].as<bool>()) {
            inputs.integral_types.insert(RULE_COMPOSITE_SIMPSONS);
        }

        if(values["use-gaussian-quadratures"].as<bool>()) {
            inputs.integral_types.insert(RULE_GAUSSIAN_QUADRATURE);
        }
    }

};
