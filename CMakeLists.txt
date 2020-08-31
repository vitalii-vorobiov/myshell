cmake_minimum_required(VERSION 3.17)
project(myshell)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.73.0 COMPONENTS program_options filesystem)

if(Boost_FOUND)
    include_directories(includes)
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(/usr/include/readline)
    add_executable(myshell main.cpp includes/EnvironmentVariables.h src/EnvironmentVariables.cpp includes/CommandLineParser.h src/CommandLineParser.cpp src/MyShell.cpp includes/MyShell.h)
    target_link_libraries(myshell ${Boost_LIBRARIES})
    target_link_libraries(myshell readline)
endif()