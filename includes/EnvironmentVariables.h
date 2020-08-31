#ifndef MYSHELL_ENVIRONMENTVARIABLES_H
#define MYSHELL_ENVIRONMENTVARIABLES_H

#include <map>
#include <string>

class EnvironmentVariables {
private:
    std::map<std::string, std::string> env_vars{};
public:
    int add_env_variable(std::string key, std::string value);
    std::string find_env_variable(std::string key);
};

#endif //MYSHELL_ENVIRONMENTVARIABLES_H
