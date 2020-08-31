#include "MyShell.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

int MyShell::merrno() {
    set_merrno(0);
    return merrno_value;
}

void MyShell::set_merrno(int value) {
    merrno_value = value;
}

int MyShell::mpwd(int argc, char **argv) {
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        MyShell::set_merrno(0);
        return 0;
    }
    std::cout << fs::current_path().string() << std::endl;
    MyShell::set_merrno(0);
    return 0;
}

int MyShell::mcd(int argc, char** argv) {
    if (argc > 2) {
        fs::current_path(argv[1]);
        return 0;
    }
    return -1;
}

int MyShell::mexit(int argc, char **argv) {
    MyShell::set_merrno(0);
    return 0;
}

int MyShell::mecho(int argc, char **argv) {
    std::cout << "MECHO" << std::endl;
    return 0;
}

int MyShell::mexport(int argc, char **argv) {
    std::cout << "MEXPORT" << std::endl;
    return 0;
}

int MyShell::run() {
    std::string buf;
    while (1)
    {
        if (buf.length() > 0)
        {
            add_history(buf.c_str());
        }

        clp.set_argv(buf);

        if (clp.get_argc() > 0)
        {
            auto prog_name = static_cast<std::string>(clp.get_program_name());

            if (prog_name == "mcd")
            {
                mcd(clp.get_argc(), clp.get_argv());
            }
            else if (prog_name == "mexit")
            {
                int res = mexit(clp.get_argc(), clp.get_argv());
                return res;
            }
            else if (prog_name == "mpwd")
            {
                mpwd(clp.get_argc(), clp.get_argv());
            }
            else if (prog_name == "mecho")
            {
                mecho(clp.get_argc(), clp.get_argv());
            }
            else if (prog_name == "mexport")
            {
                mexport(clp.get_argc(), clp.get_argv());
            }
            else if (prog_name == "meerno")
            {
                merrno();
            }
            else if (prog_name != "")
            {
                pid_t parent = getpid();
                pid_t pid = fork();
                if (pid == -1) {
                    std::cerr << "Failed to fork()" << std::endl;
                    exit(EXIT_FAILURE);
                } else if (pid > 0) {
                    // We are parent process
                    int status;
                    waitpid(pid, &status, 0);
                } else {
                    // We are the child
                    execvp(clp.get_program_name(), clp.get_argv());
                    std::cerr << "Command not found: " << clp.get_program_name() << " Code: " << errno << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    return 0;
}