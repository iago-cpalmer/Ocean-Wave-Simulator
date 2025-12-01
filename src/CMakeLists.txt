file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/arena_allocator/arena_allocator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/dear_imgui/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/dear_imgui/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/dear_imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/dear_imgui/imgui_tables.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/dear_imgui/imgui_widgets.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/dear_imgui/backends/imgui_impl_opengl3.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/dear_imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/dear_imgui/backends/misc/cpp/imgui_stdlib.*"
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories for your own code
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/arena_allocator
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/stb
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/dear_imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/dear_imgui/backends/
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
