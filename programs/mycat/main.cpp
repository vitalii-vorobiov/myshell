#include <iostream>
#include <boost/program_options.hpp>
#include <fcntl.h>

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help,h", "help")
                ("A,A", "Also prints invisible symbols with their hex code except space")
                ("input-files", po::value<std::vector<std::string>>(), "Input files")
                ;

        po::positional_options_description pd;
        pd.add("input-files", -1);
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(pd).run(), vm);
        po::notify(vm);

        std::string res;

        if (vm.count("help")) {
            std::cout << desc << "\n";
        } else if (vm.count("input-files")) {
            std::vector<std::string> files = vm["input-files"].as<std::vector<std::string>>();
            for(std::string file : files){
                int fd = open(file.c_str(), O_RDONLY);
                if (fd < 0) {
                    return 1;
                }
                off_t size = lseek(fd, 0, SEEK_END);
                lseek(fd, 0, SEEK_SET);
                char *buff = new char[size + 1];
                read(fd, buff, size);
                for (int i = 0; i < size; ++i) {
                    if (vm.count("A")) {
                        if (!isprint(buff[i])) {
                            std::stringstream str;
                            str << "0x" << std::hex << int(buff[i]);
                            res += str.str();
                        } else {
                            res += buff[i];
                        }
                    } else {
                        res += buff[i];
                    }
                }
            }
            std::cout << res << std::endl;
        }
        return 0;
    } catch (const std::exception&) {
        return 1;
    }
}