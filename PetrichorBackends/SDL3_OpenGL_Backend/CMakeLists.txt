cmake_minimum_required(VERSION 3.16)
project(SDL3_OpenGL_Backend LANGUAGES C CXX)

# Renderer API sources
set(BACKEND_SOURCES
    RendererBackend.cpp
    InputBackend.cpp
    Shader.cpp
    ShaderProgram.cpp
)

include(FetchContent)

FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG VER-2-14-1
)
FetchContent_MakeAvailable(freetype)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.4.0
)
FetchContent_MakeAvailable(SDL3)

# Create static library
add_library(SDL3_OpenGL_Backend STATIC 
  ${BACKEND_SOURCES}
  ${CMAKE_SOURCE_DIR}/vendor/glad/src/glad.c
)

target_link_libraries(SDL3_OpenGL_Backend PUBLIC freetype SDL3 PetrichorInputAPI PetrichorRendererAPI)

# Include directories for the library
target_include_directories(SDL3_OpenGL_Backend
  PUBLIC
    ${CMAKE_SOURCE_DIR}/PetrichorBackends
    ${CMAKE_SOURCE_DIR}/vendor/glad/include
)