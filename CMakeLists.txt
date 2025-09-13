cmake_minimum_required(VERSION 3.27)
project(Starter CXX)


build_application(
    SOURCES
    application.cpp
    src/core/math_utilities.cpp
    src/core/vertex_buffer.cpp
    src/core/index_buffer.cpp
    src/core/vertex_array.cpp
    src/core/shader.cpp
    src/core/event.cpp
    src/core/image.cpp
    src/core/texture.cpp
    src/core/obj_loader.cpp
    src/core/framebuffer.cpp
    src/core/shader_storage.cpp
    src/renderer/renderer.cpp
    src/widgets/widgets.cpp

    PACKAGES
    glfw3
    glm
    glad
    flecs
    stb
    tinyobjloader
    imguidocking

    LINK_PACKAGES
    glfw
    glm::glm
    glad::glad
    flecs::flecs_static
    stb::stb
    imguidocking::imguidocking
    tinyobjloader::tinyobjloader
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

generate_compile_commands()