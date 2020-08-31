#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <filesystem>

#include "myls/includes/CommandLineParser.h"
#include "myls/includes/Myls.h"


int main(int argc, char** argv) {
    myls::ProgramOptions programOptions;
    myls::CommandLineParser commandLineParser{argc, argv};
    myls::Myls myls;

    int res = commandLineParser.Parse(programOptions);
    if (res == 1) {
        return 0;
    }

    for (auto p: programOptions.paths) {
        if (exists(p)) {
            if (is_directory(p)) {
                myls.list_items_in_dir(p, programOptions);
            } else {
                auto item = directory_entry(p);
                auto filename = item.path().filename().string();

                if (programOptions.detailedFileInfo) {
                    auto size = file_size(item);
                    auto last_mod_time = last_write_time(item);
                    auto time_info = ctime(&last_mod_time);
                    cout << filename << setw(25 - filename.length()) << " " << size << setw(15 - boost::lexical_cast<string>(size).length()) << " " << time_info;
                } else {
                    cout << filename  << endl;
                }
            }
        } else {
            cerr << "The file or directory " << p << " doesn't exist!" << endl;
        }
    }

    return 0;
}
