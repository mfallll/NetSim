cmake_minimum_required(VERSION 3.27)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)

add_executable(NetSim main.cpp
        include/package.hpp
        include/types.hpp
        include/storage_types.hpp
        src/package.cpp
        src/storage_types.cpp
        test/test_package.cpp
        test/test_package.cpp)
