/**
 * @file FileParser.h
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief MISSING!
*/

#pragma once

#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <string>

#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

std::shared_ptr<std::ifstream> getFileStream(const std::string &filepath) {

    std::shared_ptr<std::ifstream> ifstreamPtr;
    try {
        // create a shared pointer input file stream
        ifstreamPtr = std::make_shared<std::ifstream>(filepath);
        // Check if the file was successfully opened
        if (!ifstreamPtr->is_open() || ifstreamPtr->fail() || ifstreamPtr->bad()) {
            throw;
        }
    } catch (std::exception &) {
        throw;
    }

    return ifstreamPtr;
}

template <typename T> void readCSV(const std::string &filepath, std::map<std::string, std::vector<T>> &data) {

    // Open the CSV file
    std::ifstream inputFile(filepath);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the CSV file." << std::endl;
        exit(1);
    }

    // Read and parse the CSV file line by line
    std::string line;
    while (std::getline(inputFile, line)) {
        // Tokenize the line using boost::tokenizer
        boost::char_separator<char> separator(",");
        boost::tokenizer<boost::char_separator<char>> tokens(line, separator);

        // Ensure there are exactly two tokens (x and f(x))
        if (std::distance(tokens.begin(), tokens.end()) != 2) {
            std::cerr << "Error: Invalid line in CSV file: " << line << std::endl;
            continue;
        }

        // Convert the tokens to long double and store them in the vector
        try {
            const auto x = boost::lexical_cast<long double>(*tokens.begin());
            const auto fx = boost::lexical_cast<long double>(*(++tokens.begin()));
            data.emplace_back(x, fx);
        } catch (const boost::bad_lexical_cast& ex) {
            std::cerr << "Error: Failed to convert to long double: " << ex.what() << std::endl;
        }
    }

    // Close the file
    inputFile.close();

    // Print the data
    for (const auto& pair : data) {
        std::cout << "x: " << pair.first << ", f(x): " << pair.second << std::endl;
    }
}
