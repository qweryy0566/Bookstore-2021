cmake_minimum_required(VERSION 3.12)
project(BookStore_2021)
set(CMAKE_CXX_FLAGS "-std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG -Wall -fsanitize=undefined -fsanitize=address")
set(SRC_FILES
        src/commands.cpp
        src/book.cpp
        src/log.cpp
        src/ull.cpp
        src/user.cpp
        src/utils.cpp
        )
add_executable(code ${SRC_FILES} src/main.cpp)