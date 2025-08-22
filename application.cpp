#include <glad/glad.h>
#include <filesystem>
#include <core/shader.hpp>
#include <GLFW/glfw3.h>
#include <print>
#include <array>
#include <vector>
#include <span>
#include <print>
#include <core/event.hpp>
#include <core/vertex_array.hpp>
#include <core/texture.hpp>
using namespace std;


int main(){
    if(!glfwInit()){
        std::print("glfwInit could not be initialized!\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    int width = 800;
    int height = 600;
    std::string title = "Hello Window";
    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* p_window, int p_width, int p_height){
        glViewport(0, 0, p_width, p_height);
    });

    // Required to setup glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::println("Could not load glad!!!");
        return -1;
    }

    // 1. Setting up hardcode shader as a string (this would be loaded as a file)

    std::array<shader_info, 2> sources = {
        shader_info{"shader_samples/triangle_shader/test.vert", shader_stage::vertex},
        shader_info{"shader_samples/triangle_shader/test.frag", shader_stage::fragment}
    };
    shader triangle_shader(sources);

    std::println("loaded = {}", triangle_shader.loaded());

    if(!triangle_shader.loaded()) {
        std::println("Triangle shader could not load one or many of the shader sources provided!!!");
    }

    // 5. load your vertices
    // opengl requires that you have your vertex buffer created with an ID
    // then a vertex array to handle vertices
    
   std::vector<float> vertices = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    std::vector<uint32_t> indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    vertex_array vao(vertices, indices);
    
    /*
    Equivalent to: (in test.vert glsl shader)
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec3 aColor;
    layout(location = 2) in vec2 aTexCoords;
    */
    std::array<vertex_attribute_element, 3> elements = {
        vertex_attribute_element{ .name = "aPos", .type = GL_FLOAT, .size = 3, },
        vertex_attribute_element{ .name = "aColor", .type = GL_FLOAT, .size = 3, },
        vertex_attribute_element{ .name = "aTexCoords", .type = GL_FLOAT, .size = 2, }
    };
    vao.vertex_attributes(elements);

    texture wood_texture(std::filesystem::path("assets/wood.png"));
    texture wall_texture(std::filesystem::path("assets/awesomeface.png"));

    triangle_shader.bind();
    triangle_shader.write("texture1", 0);
    triangle_shader.write("texture2", 1);

    while(!glfwWindowShouldClose(window)){
        // glClearColor(0.5f, 1.0f, 1.0f, 1.f);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // game logic input events

        if(is_key_pressed(window, key_q)) {
            glfwDestroyWindow(window);
        }

        // draw our first triangle
        triangle_shader.bind();
        wood_texture.bind();
        wall_texture.bind(1);
        glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        poll_events();
    }
    return 0;
}