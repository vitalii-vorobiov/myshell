#include <myls/includes/CommandLineParser.h>

std::vector<po::option> myls::CommandLineParser::files_directories_parser(std::vector<std::string> &args) {
    std::vector<po::option> paths;
    std::vector<std::string>::const_iterator i(args.begin());
    if (i != args.end() && *i == "--") {
        for (++i; i != args.end(); ++i) {
            po::option opt;
            opt.string_key = "paths";
            opt.value.push_back(*i);
            opt.original_tokens.push_back(*i);
            paths.push_back(opt);
        }
        args.clear();
    }
    return paths;
}

int myls::CommandLineParser::Parse(ProgramOptions &programOptions) {
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help,h", "Display help message")
                ("detailed-info,l", "Display detailed information about the file: name (including all extensions), size, date and time of the last modification")
                ("display-dirs-special-files,F", "Displays \"/\" in front of directories, and also specifies types of special files")
                ("reverse-sort,r", "Display a list of files in reverse order to the selected sort")
                ("recursive-dirs,R", "Go to all directories recursively, displaying their contents")
                ("sort", po::value<std::string>()->default_value("N"), "Sets the sort criteria")
                ("paths", po::value<std::vector<std::string>>(), "Name of files or directories")
                ;

        po::positional_options_description p;
        p.add("paths", -1);

        po::command_line_parser clparser(argc, argv);
        clparser.extra_style_parser(files_directories_parser);

        po::variables_map vm;
        store(clparser.options(desc).positional(p).run(), vm);
        notify(vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return 1;
        }

        if (vm.count("detailed-info")) {
            programOptions.detailedFileInfo = true;
        }
        if (vm.count("display-dirs-special-files")) {
            programOptions.specialFilesType = true;
        }
        if (vm.count("reverse-sort")) {
            programOptions.reverseSort = true;
        }
        if (vm.count("recursive-dirs")) {
            programOptions.recursive = true;
        }
        if (vm.count("sort")) {
            programOptions.sortType = vm["sort"].as<std::string>();
        }
        if (vm.count("paths")) {
            programOptions.paths = vm["paths"].as<std::vector<std::string>>();
        }
    } catch (po::error& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}