cmake_minimum_required(VERSION 3.27)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
)

add_executable(NetSim ${SOURCE_FILES} main.cpp)

set(SOURCES_FILES_TESTS
        test/test_package.cpp
)

add_executable(NetSim_test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} main.cpp)

#add_executable(NetSim main.cpp
#        include/package.hpp
#        include/types.hpp
#        include/storage_types.hpp
#        src/package.cpp
#        src/storage_types.cpp
#        test/test_package.cpp
#        test/test_package.cpp)
