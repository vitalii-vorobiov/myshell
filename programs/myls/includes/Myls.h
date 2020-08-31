#ifndef MYLS_MYLS_H
#define MYLS_MYLS_H

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <sys/stat.h>
#include <filesystem>
#include "myls/includes/CommandLineParser.h"

using namespace std;
using namespace boost::program_options;
using namespace boost::filesystem;
namespace fs = std::filesystem;

namespace myls {
    class Myls {
    private:
        static uintmax_t dirSize(directory_entry& dir);
        string get_time(directory_entry& item);
        string get_filename(directory_entry& item);

        static bool sortSize(directory_entry i1, directory_entry i2);
        static bool sortTime(directory_entry i1, directory_entry i2);
        static bool sortName(directory_entry i1, directory_entry i2);
        static bool sortExtension(directory_entry i1, directory_entry i2);
    public:
        int list_items_in_dir(path p, ProgramOptions& programOptions);
    };

}
#endif //MYLS_MYLS_H
