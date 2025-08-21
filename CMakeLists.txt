cmake_minimum_required(VERSION 3.27)
project(Starter CXX)


build_application(
    SOURCES
    application.cpp
    src/core/vertex_buffer.cpp
    src/core/index_buffer.cpp
    src/core/vertex_array.cpp
    src/core/shader.cpp
    src/core/event.cpp
    src/core/image.cpp
    src/core/texture.cpp

    PACKAGES
    glfw3
    glm
    glad
    flecs
    stb

    LINK_PACKAGES
    glfw
    glm::glm
    glad::glad
    flecs::flecs_static
    stb::stb
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

generate_compile_commands()