set (CUSTOM_LOGGER Logger/CustomLogger.cpp)

add_executable(PetrichorEditor main.cpp ${CUSTOM_LOGGER})

target_include_directories(PetrichorEditor
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(PetrichorEditor PRIVATE PetrichorEngineCore)