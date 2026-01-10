cmake_minimum_required(VERSION 3.10)
project(system-virtual-machine)

# Postavljanje C++ standarda na 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Uključivanje header fajlova
include_directories(include)

# Pronalaženje svih source fajlova u src folderu
file(GLOB SOURCES "src/*.cpp")

# Kreiranje izvršnog fajla (vm.exe)
add_executable(vm ${SOURCES})

add_custom_target(run
    COMMAND vm
    DEPENDS vm
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Poruka o uspjehu
message(STATUS "Project SVEU16_VM configured for MinGW.")