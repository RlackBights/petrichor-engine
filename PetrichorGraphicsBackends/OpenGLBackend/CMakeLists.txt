cmake_minimum_required(VERSION 3.16)
project(OpenGLBackend LANGUAGES CXX)

# Renderer API sources
set(OPENGLBACKEND_SOURCES
    OpenGLRendererBackend.cpp
)

include(FetchContent)

FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG VER-2-14-1
)

FetchContent_MakeAvailable(freetype)

# Create static library
add_library(OpenGLBackend STATIC ${OPENGLBACKEND_SOURCES})

target_link_libraries(OpenGLBackend PUBLIC freetype)

# Include directories for the library
target_include_directories(OpenGLBackend
    PUBLIC
        ${CMAKE_SOURCE_DIR}/PetrichorGraphicsBackends
)