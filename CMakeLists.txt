cmake_minimum_required(VERSION 3.17.5)
project(os)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_C_COMPILER "gcc")

add_executable(os   main.cpp
                    console.cpp
                    console.h
                    directory.cpp
                    directory.h
                    fileSystem.cpp
                    fileSystem.h
                    gcm.cpp
                    gcm.h
                    inode.cpp
                    inode.h
                    memory.cpp
                    memory.h
                    ram.cpp
                    ram.h
                    settings.h
                    user.cpp
                    user.h )