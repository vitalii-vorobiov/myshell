cmake_minimum_required(VERSION 3.17)
project(myls)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.73.0 COMPONENTS program_options filesystem)

if(Boost_FOUND)
    include_directories(includes)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(myls main.cpp src/CommandLineParser.cpp includes/Myls.h src/Myls.cpp)
    target_link_libraries(myls ${Boost_LIBRARIES})
endif()