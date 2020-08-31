#include "MyShell.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

int myshell::MyShell::merrno(int argc, char** argv) {
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help,h", "Help message");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl << "Displays merrno code" << std::endl;
            set_merrno(0);
            return 0;
        }
    } catch (po::error e) {
        set_merrno(-1);
        return -1;
    }
    std::cout << merrno_value << std::endl;
    set_merrno(0);
    return 0;
}

void myshell::MyShell::set_merrno(int value) {
    merrno_value = value;
}

int myshell::MyShell::mpwd(int argc, char** argv) {
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help,h", "Help message");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl << "Displays current path" << std::endl;
            set_merrno(0);
            return 0;
        }
        std::cout << fs::current_path().string() << std::endl;
        set_merrno(0);
        return 0;
    } catch (po::error e) {
        std::cout << e.what() << std::endl;
        set_merrno(-1);
        return -1;
    }
}

int myshell::MyShell::mcd(int argc, char** argv) {
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help,h", "Help message")
                ("path,p", "Path to change dir")
                ;

        po::positional_options_description p;
        p.add("path", 1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl << "Change directory to specified path" << std::endl;
            set_merrno(0);
            return 0;
        }

        if (vm.count("path")) {
            try {
                fs::current_path(vm["path"].as<std::string>());
                set_merrno(0);
                return 0;
            } catch (fs::filesystem_error e) {
                std::cout << e.what() << std::endl;
                set_merrno(-1);
                return -1;
            }
        }
    } catch (po::error e) {
        std::cout << e.what() << std::endl;
        set_merrno(-1);
        return -1;
    }
}

int myshell::MyShell::mexit(int argc, char **argv) {
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help,h", "Help message")
                ("code,c", po::value<int>()->default_value(0), "Exit code")
                ;

        po::positional_options_description p;
        p.add("code", 1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl << "Exit with code 0. If specified code exit with its value. If Error or invalid parameters exit with code -1" << std::endl;
            set_merrno(0);
            return 9999;
        }

        if (vm.count("code")) {
            try {
                return vm["code"].as<int>();
            } catch (boost::bad_any_cast e) {
                std::cout << e.what() << std::endl;
                return -1;
            }
        }
    } catch (po::error e) {
        std::cout << e.what() << std::endl;
        set_merrno(-1);
        return -1;
    }
}

int myshell::MyShell::mecho(int argc, char **argv) {
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help,h", "Help message")
                ("arg,a", po::value<std::vector<std::string>>(), "Arguments to echo")
                ;

        po::positional_options_description p;
        p.add("arg", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl << "Prints arguments to the console" << std::endl;
            set_merrno(0);
            return 0;
        }

        if (vm.count("arg")) {
            std::vector<std::string> arg = vm["arg"].as<std::vector<std::string>>();
            for (int i = 0; i < arg.size(); ++i) {
                std::cout << arg[i] << std::endl;
            }
        }

        set_merrno(0);
        return 0;
    } catch (po::error e) {
        std::cout << e.what() << std::endl;
        set_merrno(-1);
        return -1;
    }
}

int myshell::MyShell::run() {
    std::string buf;

    while ((buf = readline((fs::current_path().string() + " $ ").c_str())).c_str() != nullptr)
    {
        if (buf.size() > 0)
        {
            add_history(buf.c_str());
        }

        clp.set_argv(buf);

        int argc = clp.get_argc();
        char** argv = clp.get_argv();

        if (argc > 0)
        {
            auto prog_name = static_cast<std::string>(clp.get_program_name());

            if (prog_name == "mcd")
            {
                mcd(argc,argv);
            }
            else if (prog_name == "mexit")
            {
                int res = mexit(argc,argv);
                if (res != 9999) {
                    return res;
                }
            }
            else if (prog_name == "mpwd")
            {
                mpwd(argc,argv);
            }
            else if (prog_name == "mecho")
            {
                mecho(argc,argv);
            }
            else if (prog_name == "merrno")
            {
                merrno(argc, argv);
            }
            else if (prog_name != "")
            {
                pid_t parent = getpid();
                pid_t pid = fork();
                if (pid == -1) {
                    std::cerr << "Failed to fork()" << std::endl;
                    exit(EXIT_FAILURE);
                } else if (pid > 0) {
                    int status;
                    waitpid(pid, &status, 0);
                } else {
                    execvp(prog_name.c_str(), argv);
                    std::cerr << "Command not found: " << prog_name << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    return 0;
}