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
        std::cerr << "Error: Unable to open the input CSV: "<<filepath<<std::endl;
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

template <typename T> std::vector<std::string> asStringVector(const std::vector<T>& inputVector) {
    std::vector<std::string> stringVector;
    stringVector.reserve(inputVector.size()); // Reserve space for efficiency

    for (const T& value : inputVector) {
        stringVector.push_back(std::to_string(value));
    }

    return stringVector;
}

void writeCSV(const std::string &filepath, std::map<std::string, std::vector<std::string>> &data, std::vector<std::string> columns) {
    if(!isFileWritable(filepath)) {
        std::cerr<<"Error: Unable to write output CSV to path: "<<filepath<<std::endl;
        return;
    }

    // Open the CSV file for writing
    std::ofstream csvFile(filepath);

    // Check if the file was opened successfully
    if (!csvFile.is_open()) {
        std::cerr << "Error opening the output CSV file"<<filepath<< std::endl;
        return;
    }

    // Write the header row
    for(const auto &column : columns) {
        if(data.contains(column)) {
            csvFile << column << ",";
        }
    }
    csvFile << "\n";

    // Find the maximum size among all value vectors to determine the number of rows
    size_t numRows = 0;
    for (const auto &pair : data) {
        size_t currentSize = pair.second.size();
        if (currentSize > numRows) {
            numRows = currentSize;
        }
    }

    // Write the data rows
    for (size_t i = 0; i < numRows; i++) {
        for(const auto &column : columns) {
            if (data.contains(column)) {
                if (i < data[column].size()) {
                    csvFile << data[column][i];
                }
                csvFile << ",";
            }
        }
        csvFile << "\n";
    }

    // Close the CSV file
    csvFile.close();

}
