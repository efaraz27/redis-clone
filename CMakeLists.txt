cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(RedisClone CXX)

set(SRC_FILES
    src/main.cpp
    src/core/Server.cpp
    src/core/CommandHandler.cpp
)

add_executable(main ${SRC_FILES})

target_include_directories(main PUBLIC include)

set_target_properties(main PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)