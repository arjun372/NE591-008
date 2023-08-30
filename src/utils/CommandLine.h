/*
 * Arjun Earthperson
 * Helper functions for lab projects in NE591-008.
*/

#ifndef NE591_008_COMMANDLINE_H
#define NE591_008_COMMANDLINE_H

#include <string>

typedef struct  {
    std::string ProjectName;
    std::string ProjectDescription;
    std::string SubmissionDate;
    std::string StudentName;
} HeaderInfo;

class CommandLine {

public:
    explicit CommandLine(HeaderInfo &headerInfo, boost::program_options::options_description &inputs, int argc, char** argv) {

        printHeader(headerInfo);

        boost::program_options::options_description options("");
        options.add(inputs);
        options.add(buildGenerics());

        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), variablesMap);
        boost::program_options::notify(variablesMap);

        if (variablesMap.count("help")) {
            std::cout << options << "\n";
            exit(0);
        }

        if (!variablesMap.count("quiet")) {
            std::cout << options << "\n";
        }
    }

    boost::program_options::variables_map &getArguments() {
        return variablesMap;
    }
private:
    boost::program_options::options_description optionsDescription;
    boost::program_options::variables_map variablesMap;

    static boost::program_options::options_description buildGenerics() {
        boost::program_options::options_description generics("General options");
        generics.add_options()
                ("help,h", "= Show this help message")
                ("quiet,q", "= Reduce verbosity")
                ;
        return generics;
    }

    static void printHeader(HeaderInfo &headerInfo) {
        std::cout << R"(
        ███╗   ██╗███████╗    ███████╗ █████╗  ██╗
        ████╗  ██║██╔════╝    ██╔════╝██╔══██╗███║
        ██╔██╗ ██║█████╗█████╗███████╗╚██████║╚██║
        ██║╚██╗██║██╔══╝╚════╝╚════██║ ╚═══██║ ██║
        ██║ ╚████║███████╗    ███████║ █████╔╝ ██║
        ╚═╝  ╚═══╝╚══════╝    ╚══════╝ ╚════╝  ╚═╝
        )" << "\n";
        std::cout << R"(-----------------------------------------------------------)" << "\n";
        std::cout << "\t" << headerInfo.ProjectName << ": " << headerInfo.ProjectDescription << "\n";
        std::cout << "\t" << headerInfo.StudentName << "\n";
        std::cout << "\t" << headerInfo.SubmissionDate << "\n";
        std::cout << R"(-----------------------------------------------------------)" << "\n";
    }
};


#endif //NE591_008_COMMANDLINE_H
