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
        shader_info{"shader_samples/shader3_lighting1/geometry.vert", shader_stage::vertex},
        shader_info{"shader_samples/shader3_lighting1/geometry.frag", shader_stage::fragment}
    };
    // m_storage.add("mesh", mesh_sources);
    m_geometry_shader = shader(mesh_sources);

    std::array<vertex_attribute_element, 3> mesh_vertex_attributes = {
        vertex_attribute_element{ .name = "aPos", .type = GL_FLOAT, .size = 3, },
        vertex_attribute_element{ .name = "aColor", .type = GL_FLOAT, .size = 3, },
        vertex_attribute_element{ .name = "aTexCoords", .type = GL_FLOAT, .size = 2, }
    };

    std::array<shader_info, 2> lighting_shader_src = {
        shader_info{"shader_samples/shader3_lighting1/lighting.vert", shader_stage::vertex},
        shader_info{"shader_samples/shader3_lighting1/lighting.frag", shader_stage::fragment}
    };
    m_lighting_shader = shader(lighting_shader_src);

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

    // m_default_mesh = mesh(vertices, indices);
    // m_default_mesh = mesh(std::filesystem::path("assets/sphere.obj"));

    // if(m_default_mesh.loaded()) {
    //     std::println("Loaded default mesh!!!");
    // }

    // m_mesh_vao = vertex_array(vertices, indices);
    
    // /*
    // Equivalent to: (in test.vert glsl shader)
    // layout(location = 0) in vec3 aPos;
    // layout(location = 1) in vec3 aColor;
    // layout(location = 2) in vec2 aTexCoords;
    // */
    // std::array<vertex_attribute_element, 4> elements = {
    //     vertex_attribute_element{ .name = "aPos", .type = GL_FLOAT, .size = 3, },
    //     vertex_attribute_element{ .name = "aColor", .type = GL_FLOAT, .size = 3, },
    //     vertex_attribute_element{ .name = "aNormals", .type = GL_FLOAT, .size = 3, },
    //     vertex_attribute_element{ .name = "aTexCoords", .type = GL_FLOAT, .size = 2, }
    // };
    // m_default_mesh.vertex_attributes(elements);
    // // m_default_mesh.add_texture("assets/container_diffuse.png");
    // m_default_mesh.add_texture("assets/robo-pose/textures/Texture_1K.jpg");
    // m_default_mesh.add_texture("assets/robo-pose/textures/LP_BodyNormalsMap_1K.jpg");
    // m_default_mesh.add_texture("assets/robo-pose/textures/specular.jpeg");
    // m_default_mesh.add_texture("assets/robo-pose/textures/diffuse.jpeg");
}

void renderer::background_color(const glm::vec4& p_color) {
    glClearColor(p_color.x, p_color.y, p_color.z, p_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer::begin(glm::mat4 proj_view) {
    m_geometry_shader.bind();
    m_geometry_shader.write("proj_view", proj_view);
    // glm::vec3 position(0.0f, 0.0f, -3.0f);
    // glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, position);
    // // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    // m_geometry_shader.write("model", model);
}

void renderer::submit(uint64_t p_uuid, const transform* p_transform, const mesh_renderer& p_mesh_component) {
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, p_transform->position);
    std::array<vertex_attribute_element, 4> elements = {
        vertex_attribute_element{ .name = "aPos", .type = GL_FLOAT, .size = 3, },
        vertex_attribute_element{ .name = "aColor", .type = GL_FLOAT, .size = 3, },
        vertex_attribute_element{ .name = "aNormals", .type = GL_FLOAT, .size = 3, },
        vertex_attribute_element{ .name = "aTexCoords", .type = GL_FLOAT, .size = 2, }
    };

    if(!m_cached_meshes.contains(p_uuid)) {
        // TODO: Provide some API to do some invalidation to this
        // Load in the meshes
        mesh new_mesh(std::filesystem::path(p_mesh_component.model_path));
        new_mesh.vertex_attributes(elements);
        
        // Then load in and apply said textures to it
        for(size_t i = 0; i < p_mesh_component.textures_path.size(); i++) {
            new_mesh.add_texture(p_mesh_component.textures_path[i]);
        }

        if(new_mesh.loaded()) {
            m_cached_meshes.emplace(p_uuid, new_mesh);
        }
    }

    // model = glm::scale(model, p_transform->scale);
    // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    // model = glm::rotate(model, glm::radians(45.f), p_transform->rotation);
    m_geometry_shader.bind();
    m_geometry_shader.write("model", model);
}

void renderer::end() {
    // draw our first triangle
    // m_default_mesh.bind();
    // if(m_default_mesh.has_indices()) {
    //     glDrawElements(GL_TRIANGLES, (int)m_default_mesh.size(), GL_UNSIGNED_INT, nullptr);
    // }
    // else {
    //     glDrawArrays(GL_TRIANGLES, 0, 36);
    // }
    for(const auto&[uuid, mesh] : m_cached_meshes) {
        if(mesh.has_indices()) {
        glDrawElements(GL_TRIANGLES, (int)mesh.size(), GL_UNSIGNED_INT, nullptr);
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
}