/**
 * @file FileParser.h
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief MISSING!
*/

#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <string>

#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

static bool isFileWritable(const std::string &filepath) {
    const std::filesystem::path path(filepath);
    std::error_code ec; // For using the non-throwing overloads of functions below.
    if(exists(path,ec)) {
        std::cerr << "Error: File already exists at path. "<<std::endl;
        return false;
    }
    return true;
}

template <typename T> void readCSV(const std::string &filepath, std::map<std::string, std::vector<T>> &data) {

    // Open the CSV file
    std::ifstream inputFile(filepath);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the CSV file." << std::endl;
        exit(1);
    }
    // Read the header row to get column names
    std::string header;
    if (std::getline(inputFile, header)) {
        // Tokenize the header row using boost::tokenizer
        boost::char_separator<char> separator(",");
        boost::tokenizer<boost::char_separator<char>> headerTokens(header, separator);

        // Initialize the map with column names as keys
        for (const auto& columnName : headerTokens) {
            data[columnName].clear(); // Clear any existing data for this column
        }

        // Read and parse the data rows
        std::string line;
        while (std::getline(inputFile, line)) {
            // Tokenize the line using boost::tokenizer
            boost::tokenizer<boost::char_separator<char>> tokens(line, separator);

            // Iterate through the tokens and add them to the corresponding column vectors
            auto columnIter = data.begin();
            for (const auto& token : tokens) {
                if (columnIter == data.end()) {
                    std::cerr << "Error: More columns in data than in the header." << std::endl;
                    return;
                }

                try {
                    columnIter->second.push_back(boost::lexical_cast<long double>(token));
                } catch (const boost::bad_lexical_cast& ex) {
                    std::cerr << "Error: Failed to convert to long double: " << ex.what() << std::endl;
                }

                ++columnIter;
            }

            if (columnIter != data.end()) {
                std::cerr << "Error: More columns in the header than in the data." << std::endl;
                return;
            }
        }
    } else {
        std::cerr << "Error: CSV file is empty." << std::endl;
    }

    // Close the file
    inputFile.close();
}
