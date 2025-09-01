#pragma once
#include <glm/glm.hpp>
#include <string>
#include <core/vertex_array.hpp>
#include <array>
#include <core/test_camera.hpp>
#include <core./texture.hpp>
#include <core/shader.hpp>
// class mesh_metadata {
// public:
//     mesh_metadata() = default;
//     mesh_metadata(const std::filesystem::path& p_path, const std::span<std::string>& p_texture_slots) {
//         invalidate(p_path, p_texture_slots);
//     }

//     void bind() {
//         m_vbo.bind();
//         if(m_ibo.size() > 0) {
//             m_ibo.bind();
//         }
//     }

//     void unbind() {
//         m_vbo.unbind();
//         if(m_ibo.size() > 0) {
//             m_ibo.unbind();
//         }
//     }

//     void invalidate(const std::filesystem::path& p_path, const std::span<std::string>& p_texture_slots) {
//         // load .obj model =>  sets up vertex/index buffers

//         obj_loader model(p_path.string());
//         m_vbo = vertex_buffer(model.vertices());
//         m_ibo = index_buffer(model.indices());
//     }

//     [[nodiscard]] uint64_t indices_count() const { return m_ibo.size(); }

// private:
//     // vertex_array m_mesh_vao; // loads in both vertex/index buffers and handle the vertex attributes
//     vertex_buffer m_vbo;
//     index_buffer m_ibo;
// };

class renderer {
public:
    renderer(const std::string& p_name);

    void background_color(const glm::vec4& p_color);

    // void begin(const glm::mat4& p_proj_view);
    void begin(glm::mat4 proj_view);


    // cached draw calls

    // void draw_mesh(uint64_t p_uuid, const transform* p_transform, const mesh_renderer& p_user_mesh);


    void end();


private:
    vertex_array m_mesh_vao;
    std::array<vertex_attribute_element, 3> m_mesh_vertex_attributes;
    glm::mat4 m_model{1.f};

    shader m_triangle_shader;

    texture m_wood;
    texture m_wall;
};