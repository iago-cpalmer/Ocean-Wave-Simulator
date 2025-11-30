file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/arena_allocator/arena_allocator.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories for your own code
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/arena_allocator
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/stb
)

# Link vendor libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glfw
        assimp
	    glm
        glad
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
)
