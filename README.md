# CppDummyAPI
A dummy REST API in C++ to show that making a REST API can be really easy, even in C++. In this repository you have the sources and several ways to build this project.

This project illustrate an article I am writting on my blog.

Note that the API is far from optimal, but I made it this way to show the possibility of __cpp-httplib__.

## Depedencies
I used 2 awesome libraries :
* __[nlohmann/json](https://github.com/nlohmann/json)__
* __[cpp-httplib](https://github.com/yhirose/cpp-httplib)__

Their sources are included in the repository, but the __CMakeLists.txt__ can also use several package managers to retrieve them.

## How to build
### Command line
Just run the script `build.sh`. It will call g++ with required arguments. (It won't work on windows)

### CMake without package manager
* `cmake -B build`
* `cmake --build build`

### CMake with CPM.cmake
Run the following commands:
* `cmake -B build -DPACKAGE_MANAGER=CPM`
* `cmake --build build`

### CMake with Vcpkg
Run the following commands where you replace \[VCPKG_LOCATION] by the path to where vcpkg is installed:
* `cmake -B build -DCMAKE_TOOLCHAIN_FILE=[VCPKG_LOCATION]/vcpkg/scripts/buildsystems/vcpkg.cmake -DPACKAGE_MANAGER=VCPKG`
* `cmake --build build`

### CMake with conan
Run the following commands:
* `conan install . -if build`
* `cmake -B build -DPACKAGE_MANAGER=CONAN`
* `cmake --build build`