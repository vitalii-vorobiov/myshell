cmake_minimum_required(VERSION 3.15)
project(mycat)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(mycat main.cpp)
target_link_libraries(mycat LINK_PUBLIC ${Boost_LIBRARIES})
