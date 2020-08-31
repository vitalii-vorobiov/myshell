#include <iostream>
#include "CommandLineParser.h"

int myshell::CommandLineParser::set_argv(std::string str) {
    argv.clear();
    bool isQuoted;
    std::string token;
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] == '"')
        {
            isQuoted = !isQuoted;
            if(token.length() > 0)
            {
                argv.push_back(token);
                token.clear();
            }
        }
        else if (str[i] != ' ')
        {
            token.push_back(str[i]);
            if (i == str.size() - 1)
            {
                argv.push_back(token);
                token.clear();
            }
        }
        else
        {
            if (isQuoted)
            {
                token.push_back(str[i]);
            }
            else if(token.length() > 0)
            {
                argv.push_back(token);
                token.clear();
            }
        }
    }
    return 0;
}

char** myshell::CommandLineParser::get_argv() {
    p.clear();
    p = std::vector<char*>(argv.size()+1);
    for(size_t i = 0; i < argv.size(); ++i)
    {
        p[i] = argv[i].data();
    }
    p[argv.size()] = NULL;
    return p.data();
}

int myshell::CommandLineParser::get_argc() {
    return argv.size();
}

char* myshell::CommandLineParser::get_program_name() {
    if (argv.size() > 0) {
        return argv[0].data();
    }

    return nullptr;
}