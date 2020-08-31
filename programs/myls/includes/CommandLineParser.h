#ifndef MYLS_COMMANDLINEPARSER_H
#define MYLS_COMMANDLINEPARSER_H

#include <vector>
#include <string>
#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace myls {
    struct ProgramOptions {
        bool detailedFileInfo = false; // -l
        bool specialFilesType = false; // -F
        bool reverseSort = false; // -r
        bool recursive = false; // -R
        std::string sortType = "N"; // U S t X N | D s
        std::vector<std::string> paths = {"."};
    };

    class CommandLineParser {
    private:
        static std::vector<po::option> files_directories_parser(std::vector<std::string>& args);
    public:
        int argc;
        char** argv;

        int Parse(ProgramOptions &programOptions);
    };
}

#endif //MYLS_COMMANDLINEPARSER_H
