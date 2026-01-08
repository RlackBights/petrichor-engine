cmake_minimum_required(VERSION 3.16)
project(PetrichorRendererAPI LANGUAGES CXX)

# Renderer API sources
set(RENDERERAPI_SOURCES
    NullRendererBackend.cpp
    Renderer.cpp
    Text/FontLoader.cpp
    Text/Font.cpp
)

# Create static library
add_library(PetrichorRendererAPI STATIC ${RENDERERAPI_SOURCES})

# Include directories for the library
target_include_directories(PetrichorRendererAPI
    PUBLIC
        ${CMAKE_SOURCE_DIR}/PetrichorBackends
)