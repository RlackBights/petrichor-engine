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

# Input source files
# None for now

# IO source files
set (IO
    IO/FileProcessor.cpp
    IO/MeshLoader.cpp
    IO/OBJMeshLoader.cpp
)

# JSON source files
set(JSON JSON/JSON.cpp)

# Math source files
set(MATH Math/Math.cpp)

# Rendering source fiels
set(RENDERING
    Rendering/MeshRenderer.cpp
    Rendering/MeshFilter.cpp
)

# Scene source files
set(SCENE
    Scene/Camera.cpp
    Scene/Transform.cpp
)

# Text source files
# None for now

# Time source files
set(TIME Time/Time.cpp)

# Engine sources grouped together
set(ENGINE_SOURCES
    ${CORE}
    ${DEBUG}
    ${ECS}
    ${IO}
    ${JSON}
    ${MATH}
    ${RENDERING}
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