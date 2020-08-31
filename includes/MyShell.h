#ifndef MYSHELL_MYSHELL_H
#define MYSHELL_MYSHELL_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "CommandLineParser.h"

class MyShell {
private:
    int merrno_value;
    CommandLineParser clp{};
public:
    int merrno();
    void set_merrno(int value);
    int mpwd(int argc, char **argv);
    int mcd(int argc, char** argv);
    int mexit(int argc, char** argv);
    int mecho(int argc, char **argv);
    int mexport(int argc, char **argv);
    int run();
};

#endif //MYSHELL_MYSHELL_H
