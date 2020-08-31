#include "myls/includes/Myls.h"

uintmax_t myls::Myls::dirSize(directory_entry &dir) {
    uintmax_t size = 0;
    for(recursive_directory_iterator it(dir); it!=recursive_directory_iterator(); ++it) {
        if (!is_directory(*it) && exists(*it) ) { size += file_size(*it); }
    }
    return size;
}

string myls::Myls::get_time(directory_entry &item) {
    auto last_mod_time = last_write_time(item);
    auto time_info = ctime(&last_mod_time);
    if (time_info[strlen(time_info)-1] == '\n') time_info[strlen(time_info)-1] = '\0';
    return time_info;
}

string myls::Myls::get_filename(directory_entry& item) {
    return item.path().filename().string();
}

bool myls::Myls::sortSize(directory_entry i1, directory_entry i2)
{
    uintmax_t size1, size2;
    if (is_directory(i1)) {
        size1 = dirSize(i1);
    } else {
        size1 = file_size(i1);
    }

    if (is_directory(i2)) {
        size2 = dirSize(i2);
    } else {
        size2 = file_size(i2);
    }
    return (size1 < size2);
}

bool myls::Myls::sortTime(directory_entry i1, directory_entry i2)
{
    return (last_write_time(i1) < last_write_time(i2));
}

bool myls::Myls::sortName(directory_entry i1, directory_entry i2)
{
    return (i1.path().stem().string() < i2.path().stem().string());
}

bool myls::Myls::sortExtension(directory_entry i1, directory_entry i2)
{
    return (i1.path().extension() < i2.path().extension());
}

int myls::Myls::list_items_in_dir(path p, ProgramOptions& programOptions) {
    vector<directory_entry> dir_entries;
    for (auto & item: directory_iterator(p)) {
        dir_entries.push_back(item);
    }

    if (programOptions.sortType == "U") {

    } else if (programOptions.sortType == "S") {
        sort(dir_entries.begin(), dir_entries.end(), sortSize);
    } else if (programOptions.sortType == "t") {
        sort(dir_entries.begin(), dir_entries.end(), sortTime);
    } else if (programOptions.sortType == "X") {
        sort(dir_entries.begin(), dir_entries.end(), sortExtension);
    } else if (programOptions.sortType == "N") {
        sort(dir_entries.begin(), dir_entries.end(), sortName);
    }

    if (programOptions.reverseSort) {
        reverse(dir_entries.begin(), dir_entries.end());
    }

    cout << "Path: " << p.string() << endl;
    for(auto & item: dir_entries) {
        try {
            if (is_directory(item)) {
                if (programOptions.detailedFileInfo) {
                    uintmax_t size = 0;
                    for(recursive_directory_iterator it(item); it!=recursive_directory_iterator(); ++it) {
                        if (!is_directory(*it) && exists(*it) ) { size += file_size(*it); }
                    }
                    cout << size << setw(16 - boost::lexical_cast<string>(size).length()) << " " << get_time(item) << " ";
                }
                cout << "/" << get_filename(item) << endl;
            } else {
                if (programOptions.detailedFileInfo) {
                    auto size = file_size(item);
                    cout << size << setw(16 - boost::lexical_cast<string>(size).length()) << " " << get_time(item) << " ";
                }
                if (programOptions.specialFilesType) {
                    if (fs::is_fifo(item.path().c_str())) cout << "|";
                    if (fs::is_symlink(item.path().c_str())) cout << "@";
                    if (fs::is_socket(item.path().c_str())) cout << "=";
                    struct stat st;
                    if (stat(item.path().c_str(), &st) >= 0) {
                        if ((st.st_mode & S_IEXEC) != 0) {
                            cout << "*";
                        }
                    }
                }
                cout << get_filename(item) << endl;
            }
        } catch (const filesystem_error& e) {
            cerr << e.what() << endl;
        }
    }
    cout << endl;

    if (programOptions.recursive) {
        for(auto & item: dir_entries) {
            if (is_directory(item)) {
                list_items_in_dir(item.path(), programOptions);
            }
        }
    }
    return 0;
}