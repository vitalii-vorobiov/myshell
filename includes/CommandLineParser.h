#ifndef MYSHELL_COMMANDLINEPARSER_H
#define MYSHELL_COMMANDLINEPARSER_H

#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace myshell {
    class CommandLineParser {
    private:
        std::vector<std::string> argv;
        std::vector<char*> p;
    public:
        int set_argv(std::string str);
        char** get_argv();
        int get_argc();
        char* get_program_name();
    };
}

#endif //MYSHELL_COMMANDLINEPARSER_H
