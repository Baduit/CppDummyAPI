cmake_minimum_required(VERSION 3.9)

project(CppDummyAPI VERSION 1.0.0 DESCRIPTION "A dummy projet to show that making a REST API in C++ is not difficult.")

find_package(Threads REQUIRED)

option(PACKAGE_MANAGER "PACKAGE_MANAGER" "NONE")

add_executable(api ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)
target_compile_features(api PRIVATE cxx_std_14)
target_compile_options(api
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<CXX_COMPILER_ID:GNU>:-Wextra -Wall -Wsign-conversion -pedantic -Wredundant-decls -Wpointer-arith -O2>)

target_link_libraries(api PRIVATE Threads::Threads)
target_include_directories(api PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)