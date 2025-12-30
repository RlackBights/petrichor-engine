# Core source files
set(CORE
    Core/ConsoleLogger.cpp
    Core/Log.cpp
)

# Debug source files
set(DEBUG Debug/Debug.cpp)

# ECS source files
set(ECS 
    ECS/Component.cpp
    ECS/Object.cpp
)

# JSON source files
set(JSON JSON/JSON.cpp)

# Math source files
# None for now

# Rendering source fiels
# None for now

# Scene source files
set(SCENE
    Scene/Camera.cpp
)

# Time source files
set(TIME Time/Time.cpp)

# Engine sources grouped together
set(ENGINE_SOURCES
    ${CORE}
    ${DEBUG}
    ${ECS}
    ${JSON}
    ${SCENE}
    ${TIME}
)

# Create static library
add_library(PetrichorEngineCore STATIC ${ENGINE_SOURCES})

# Include directories for the library
target_include_directories(PetrichorEngineCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)