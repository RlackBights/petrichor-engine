cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(petrichor_engine C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")


set(CMAKE_INSTALL_RPATH "$ORIGIN/libs")
set(CMAKE_BUILD_RPATH "$ORIGIN/libs")
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_SKIP_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

if(WIN32)
    add_compile_options(/EHsc)
endif()

# Vendored Dependencies
# =========================
add_subdirectory(vendor/glm)
add_subdirectory(vendor/SDL3-3.2.14)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/freetype-2.13.3)
find_package(OpenGL REQUIRED)

# =========================
# Engine and Game Modules
# =========================

file(GLOB ENGINE_LIB_SRC "engine/src/*.cpp")
file(GLOB GAME_LIB_SRC "scripts/src/*.cpp")
file(GLOB GAME_STD_SRC "scripts/standalone/*.cpp")
file(GLOB GAME_ENTRY_SRC "scripts/game_main.cpp")

add_library(LibsModule STATIC
    ${ENGINE_LIB_SRC}
    ${GAME_LIB_SRC}
    ${GAME_STD_SRC}
    ${GAME_ENTRY_SRC}
)

target_include_directories(LibsModule PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor/freetype-2.13.3
    ${CMAKE_SOURCE_DIR}/vendor/freetype-2.13.3/include
    ${CMAKE_SOURCE_DIR}/vendor/stb
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/scripts/include
)

target_link_libraries(LibsModule
    glm
    SDL3::SDL3
    glad
    OpenGL::GL
    freetype
)

# =========================
# Engine Executable
# =========================

add_executable(petrichor_engine main.cpp)
target_link_libraries(petrichor_engine LibsModule)

# =========================
# Resource Copy (Post-Build)
# =========================

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_SOURCE_DIR}/bin/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_SOURCE_DIR}/bin/resources
    COMMAND ${CMAKE_SOURCE_DIR}/update_libraries.sh
    COMMENT "Copying resources and updating libraries"
)
add_dependencies(copy_resources petrichor_engine)
