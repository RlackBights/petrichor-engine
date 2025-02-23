cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${directory})

project(petrichor_engine C CXX)

include(FetchContent)

find_package(SDL3 QUIET)
if (NOT SDL3_FOUND)
    message(STATUS "SDL3 not found, fetching from source...")
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.0.0
    )
    FetchContent_MakeAvailable(SDL3)
endif()

find_package(OpenGL QUIET)
if (NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL is required but not found! Install it manually.")
endif()

find_package(Freetype QUIET)
if (NOT Freetype_FOUND)
    message(STATUS "Freetype not found, fetching from source...")
    FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
        GIT_TAG VER-2-13-3
    )
    FetchContent_MakeAvailable(freetype)
endif()

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG c
    SOURCE_SUBDIR cmake
)
FetchContent_MakeAvailable(glad)

FetchContent_Declare(
    stb_image
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb_image)

include_directories(${glad_SOURCE_DIR}/include)
include_directories(${stb_image_SOURCE_DIR})
include_directories("engine/include/")

file(GLOB MY_LIB_SRC "engine/src/*.cpp")
file(GLOB GLAD_SRC "${glad_SOURCE_DIR}/src/glad.c")

add_library(LibsModule STATIC ${MY_LIB_SRC} ${GLAD_SRC})

target_link_libraries(LibsModule SDL3::SDL3 OpenGL::GL Freetype::Freetype)

add_executable(petrichor_engine main.cpp)

add_custom_command(
    TARGET petrichor_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_SOURCE_DIR}/bin/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_SOURCE_DIR}/bin/resources
)

target_link_libraries(petrichor_engine LibsModule)

