#include <renderer/renderer.hpp>
#include <print>
#include <glad/glad.h>
#include <array>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

renderer::renderer(const std::string& p_name) {
    std::array<shader_info, 2> mesh_sources = {
        shader_info{"shader_samples/shader2_camera/test.vert", shader_stage::vertex},
        shader_info{"shader_samples/shader2_camera/test.frag", shader_stage::fragment}
    };
    // m_storage.add("mesh", mesh_sources);
    m_triangle_shader = shader(mesh_sources);

    std::array<vertex_attribute_element, 3> mesh_vertex_attributes = {
        vertex_attribute_element{ .name = "aPos", .type = GL_FLOAT, .size = 3, },
        vertex_attribute_element{ .name = "aColor", .type = GL_FLOAT, .size = 3, },
        vertex_attribute_element{ .name = "aTexCoords", .type = GL_FLOAT, .size = 2, }
    };

    // setting up vao
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

    m_mesh_vao = vertex_array(vertices, indices);
    
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
    m_mesh_vao.vertex_attributes(elements);

    m_wood = texture(std::filesystem::path("assets/wood.png"));
    m_wall = texture(std::filesystem::path("assets/awesomeface.png"));

    m_triangle_shader.bind();
    m_triangle_shader.write("texture1", 0);
    m_triangle_shader.write("texture2", 1);
}

void renderer::background_color(const glm::vec4& p_color) {
    glClearColor(p_color.x, p_color.y, p_color.z, p_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer::begin(camera p_camera, float p_aspect_ratio) {
    glm::mat4 projection = glm::perspective(glm::radians(p_camera.Zoom), p_aspect_ratio, 0.1f, 100.0f);
    glm::mat4 view = p_camera.GetViewMatrix();

    // // test_vec = model * test_vec;
    m_triangle_shader.write("projection", projection);
    m_triangle_shader.write("view", view);
    

    glm::vec3 position(0.0f, 0.0f, -3.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    m_triangle_shader.write("model", model);
}

// void renderer::draw_mesh(uint64_t p_uuid, const transform* p_transform, const mesh_renderer& p_user_mesh) {
//     m_model = glm::mat4(1.f);
//     m_model = glm::translate(m_model, p_transform->position);
//     m_model = glm::scale(m_model, p_transform->scale);
// }

void renderer::end() {

    // draw our first triangle
    m_triangle_shader.bind();
    m_wood.bind();
    m_wall.bind(1);
    glDrawElements(GL_TRIANGLES, m_mesh_vao.size(), GL_UNSIGNED_INT, nullptr);
}