#pragma once
#include <glm/glm.hpp>
#include <string>
#include <core/vertex_array.hpp>
#include <core./texture.hpp>
#include <renderer/components.hpp>
#include <core/vertex_array.hpp>
#include <unordered_map>
#include <core/shader_storage.hpp>

class mesh {
public:
    mesh() = default;
    mesh(const std::span<float>& p_vertices, const std::span<uint32_t>& p_indices) {
        m_vao = vertex_array(p_vertices, p_indices);
        m_loaded = m_vao.loaded();
    }

    mesh(const std::filesystem::path& p_path) {
        m_vao = vertex_array(p_path);
        m_loaded = m_vao.loaded();
    }


    [[nodiscard]] bool loaded() const { return m_loaded; }

    void add_texture(const std::string& p_path) {
        texture t(p_path);
        if(!t.loaded()) {
            std::println("Could not load path {}", p_path);
            return;
        }
        m_textures.emplace_back(t);
    }

    void vertex_attributes(const std::span<vertex_attribute_element>& p_attributes) {
        m_vao.vertex_attributes(p_attributes);
    }

    [[nodiscard]] std::span<const texture> texture_data() const { return m_textures; }

    [[nodiscard]] bool has_indices() const {
        return m_vao.has_indices();
    }

    void bind_textures() {
        // m_vao.bind();

        for(size_t i = 0; i < m_textures.size(); i++) {
            m_textures[i].bind(i);
        }
    }

    void unbind_textures() {
        for(size_t i = 0; i < m_textures.size(); i++) {
            m_textures[i].unbind();
        }
    }

    void bind_vao() {
        m_vao.bind();
    }

    void unbind_vao() {
        m_vao.unbind();
    }

    [[nodiscard]] uint32_t size() const { return m_vao.size(); }

private:
    vertex_array m_vao;
    std::vector<texture> m_textures;
    bool m_loaded=false;
};

class renderer {
public:
    renderer(const std::string& p_name);

    void background_color(const glm::vec4& p_color);

    // void begin(const glm::mat4& p_proj_view);
    void begin(glm::mat4 proj_view);

    // void draw_mesh(uint64_t p_uuid, const transform* p_transform, const mesh_renderer& p_user_mesh);

    //! @brief Every submission of a task is to be operated whenever an object has these specific component
    //! @brief Then that work will be done and prepared before the the draw call gets done
    void submit(uint64_t p_uuid, const transform* p_transform, const mesh_renderer& p_mesh_component);


    void end();


private:
    // vertex_array m_mesh_vao;
    std::unordered_map<uint64_t, mesh> m_cached_meshes;

    shader_storage m_shader_storage;

    // shader m_geometry_shader;
    // shader m_lighting_shader;

    
    // mesh m_default_mesh;
};