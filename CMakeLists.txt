cmake_minimum_required(VERSION 3.9)

project(CppDummyAPI VERSION 1.0.0 DESCRIPTION "A dummy projet to show that making a REST API in C++ is not difficult.")


set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Threads REQUIRED)

add_executable(api ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)
target_compile_features(api PRIVATE cxx_std_17)
target_compile_options(api
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<CXX_COMPILER_ID:GNU>:-Wextra -Wall -Wsign-conversion -pedantic -Wredundant-decls -Wpointer-arith -O2>)

option(PACKAGE_MANAGER "PACKAGE_MANAGER" "NONE")
if (PACKAGE_MANAGER STREQUAL "NONE")                       
    target_link_libraries(api PRIVATE Threads::Threads)
    target_include_directories(api PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

elseif(PACKAGE_MANAGER STREQUAL "VCPKG")
    # Find and link nlohmann_json 
    find_package(nlohmann_json REQUIRED)
    target_link_libraries(api PRIVATE Threads::Threads nlohmann_json::nlohmann_json)

    # Find and add to the path httplib.h
    find_path(CPP_HTTPLIB_INCLUDE_DIRS "httplib.h")
    target_include_directories(api PRIVATE ${CPP_HTTPLIB_INCLUDE_DIRS})

elseif(PACKAGE_MANAGER STREQUAL "CPM")
    include(CPM)

    # Retrieve nlohmann_json
    CPMAddPackage(
        NAME nlohmann_json
        VERSION 3.9.1
        # the git repo is incredibly large, so we download the archived include directory
        URL https://github.com/nlohmann/json/releases/download/v3.9.1/include.zip
        URL_HASH SHA256=6bea5877b1541d353bd77bdfbdb2696333ae5ed8f9e8cc22df657192218cad91
    )
  
    # Create a library from the sources
    if (nlohmann_json_ADDED)
        add_library(nlohmann_json INTERFACE IMPORTED)
        target_include_directories(nlohmann_json INTERFACE ${nlohmann_json_SOURCE_DIR}/include)
    endif()

    # Link the json library
    target_link_libraries(api PRIVATE Threads::Threads nlohmann_json)

    
    CPMAddPackage(
        NAME cpp_httplib
        GITHUB_REPOSITORY  yhirose/cpp-httplib
        VERSION 0.8.7
    )
    target_include_directories(api PRIVATE ${cpp_httplib_SOURCE_DIR})

elseif(PACKAGE_MANAGER STREQUAL "CONAN")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
    target_link_libraries(api PRIVATE ${CONAN_LIBS})

else()
    message(SEND_ERROR ${PACKAGE_MANAGER} " is not supported")
endif()