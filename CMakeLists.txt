cmake_minimum_required(VERSION 3.20)
project (HelloMpi)

set(CMAKE_CXX_STANDARD 14)

# Add MPI Package to Project
find_package(MPI REQUIRED)

add_executable(HelloMpi main.cpp)
# Add libraries for code completion and compiling
target_link_libraries(HelloMpi PUBLIC MPI::MPI_CXX)