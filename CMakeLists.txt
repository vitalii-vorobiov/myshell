cmake_minimum_required(VERSION 3.17)
project(myshell)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS program_options filesystem REQUIRED)
find_library(Readline_LIBRARY NAMES readline)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Can't find boost library")
endif()

include_directories(includes)
include_directories(programs)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(/usr/include/readline)

add_executable(myshell main.cpp src/CommandLineParser.cpp src/MyShell.cpp)
target_link_libraries(myshell ${Boost_LIBRARIES})
target_link_libraries(myshell readline)

add_executable(mycat programs/mycat/main.cpp)
target_link_libraries(mycat ${Boost_LIBRARIES})

add_executable(myls programs/myls/main.cpp programs/myls/src/CommandLineParser.cpp programs/myls/src/Myls.cpp)
target_link_libraries(myls ${Boost_LIBRARIES})