#pragma once
#include <glm/glm.hpp>
#include <string>
#include <core/vertex_array.hpp>
#include <array>
#include <core/test_camera.hpp>
#include <core./texture.hpp>
#include <core/shader.hpp>
#include <renderer/components.hpp>
#include <core/vertex_array.hpp>

class mesh {
public:
    mesh() = default;
    mesh(const std::span<float>& p_vertices, const std::span<uint32_t>& p_indices) {
        m_vao = vertex_array(p_vertices, p_indices);
    }

    void add_texture(const std::string& p_path) {
        m_textures.emplace_back(p_path);
    }

    void vertex_attributes(const std::span<vertex_attribute_element>& p_attributes) {
        m_vao.vertex_attributes(p_attributes);
    }

    [[nodiscard]] std::span<const texture> texture_data() const { return m_textures; }

    void bind() {
        m_vao.bind();

        for(size_t i = 0; i < m_textures.size(); i++) {
            m_textures[i].bind(i);
        }
    }

    [[nodiscard]] uint32_t size() const { return m_vao.size(); }

private:
    vertex_array m_vao;
    std::vector<texture> m_textures;
};

class renderer {
public:
    renderer(const std::string& p_name);

    void background_color(const glm::vec4& p_color);

    // void begin(const glm::mat4& p_proj_view);
    void begin(glm::mat4 proj_view);

    // void draw_mesh(uint64_t p_uuid, const transform* p_transform, const mesh_renderer& p_user_mesh);

    void draw(const transform* p_transform, const mesh_renderer& p_mesh_component);


    void end();


private:
    vertex_array m_mesh_vao;
    std::array<vertex_attribute_element, 3> m_mesh_vertex_attributes;
    // glm::mat4 m_model{1.f};

    shader m_triangle_shader;

    mesh m_default_mesh;

    // texture m_wood;
    // texture m_wall;
};