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
    std::array<shader_info, 2> geometry_shaders = {
        shader_info{"shader_samples/shader3_lighting1/geometry.vert", shader_stage::vertex},
        shader_info{"shader_samples/shader3_lighting1/geometry.frag", shader_stage::fragment}
    };
    m_shader_storage.load_shader(shader_type::geometry, geometry_shaders);

    std::array<vertex_attribute_element, 3> mesh_vertex_attributes = {
        vertex_attribute_element{ .name = "aPos", .type = GL_FLOAT, .size = 3, },
        vertex_attribute_element{ .name = "aColor", .type = GL_FLOAT, .size = 3, },
        vertex_attribute_element{ .name = "aTexCoords", .type = GL_FLOAT, .size = 2, }
    };

    std::array<shader_info, 2> lighting_shaders = {
        shader_info{"shader_samples/shader3_lighting1/lighting.vert", shader_stage::vertex},
        shader_info{"shader_samples/shader3_lighting1/lighting.frag", shader_stage::fragment}
    };
    m_shader_storage.load_shader(shader_type::lighting, lighting_shaders);
}

void renderer::background_color(const glm::vec4& p_color) {
    glClearColor(p_color.x, p_color.y, p_color.z, p_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer::begin(glm::mat4 proj_view) {
    auto geo_shader = m_shader_storage.get(shader_type::geometry);
    geo_shader->bind();
    geo_shader->write("proj_view", proj_view);
}

void renderer::submit(uint64_t p_uuid, const transform* p_transform, const mesh_renderer& p_mesh_component) {
    auto geo_shader = m_shader_storage.get(shader_type::geometry);
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, p_transform->position);
    model = glm::scale(model, p_transform->scale);
    // TODO: Add this back in later
    // glm::mat4 rotation_mat4 =
    //           glm::mat4(glm::quat(p_transform->rotation));

    // model *= rotation_mat4;

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
            std::println("Loading texture path = {}", p_mesh_component.textures_path[i]);
            new_mesh.add_texture(p_mesh_component.textures_path[i]);
        }

        if(new_mesh.loaded()) {
            m_cached_meshes.emplace(p_uuid, new_mesh);
        }
    }
    geo_shader->bind();
    geo_shader->write("model", model);
}

void renderer::end() {
    for(auto& [uuid, mesh] : m_cached_meshes) {
        mesh.bind_vao();
        mesh.bind_textures();
        if(mesh.has_indices()) {
            glDrawElements(GL_TRIANGLES, (int)mesh.size(), GL_UNSIGNED_INT, nullptr);
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

    }
}