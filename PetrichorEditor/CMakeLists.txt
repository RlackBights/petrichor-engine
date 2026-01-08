set (GUI GUI/GUI.cpp)

set (EDITOR_SOURCES
    ${GUI}
)

add_executable(PetrichorEditor main.cpp ${EDITOR_SOURCES})

target_include_directories(PetrichorEditor
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(PetrichorEditor PRIVATE PetrichorEngineCore SDL3_OpenGL_Backend)

add_custom_target(CopyResources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/PetrichorEditor/Resources"
            "$<TARGET_FILE_DIR:PetrichorEditor>/Resources"
    COMMENT "Copying resources to build folder"
)
