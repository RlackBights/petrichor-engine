cmake_minimum_required(VERSION 3.16)
project(PetrichorInputAPI LANGUAGES CXX)

# Renderer API sources
set(INPUTAPI_SOURCES
    InputManager.cpp
    Keybinds.cpp
    NullInputBackend.cpp
)

# Create static library
add_library(PetrichorInputAPI STATIC ${INPUTAPI_SOURCES})

# Include directories for the library
target_include_directories(PetrichorInputAPI
    PUBLIC
        ${CMAKE_SOURCE_DIR}/PetrichorBackends
)