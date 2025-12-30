cmake_minimum_required(VERSION 3.16)
project(ClassNamingCheckTool LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define tool source
add_executable(ClassNamingCheckTool main.cpp)

# Link necessary Clang and LLVM libraries
target_link_libraries(ClassNamingCheckTool
    PRIVATE
    LLVM
    clang-cpp
)