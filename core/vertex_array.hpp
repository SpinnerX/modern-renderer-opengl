#pragma once
#include <core/vertex_buffer.hpp>
#include <core/index_buffer.hpp>

class vertex_array {
public:
    vertex_array() = default;
    // vertex_array(const std::span<float>& p_vertices);
    vertex_array(const std::span<float>& p_vertices, const std::span<uint32_t>& p_indices);
    // vertex_array(const std::span<vertex>& p_vertices);
    // vertex_array(const std::span<vertex>& p_vertices, const std::span<uint32_t>& p_indices);

    void bind();

    void unbind();

    void vertex_attributes(const std::span<vertex_attribute_element>& p_attribute_elements);

    [[nodiscard]] bool has_indices() const  { return (m_ibo.size() > 0); }

    [[nodiscard]] int size() const { return (int)m_ibo.size(); }

private:
    vertex_buffer m_vbo;
    index_buffer m_ibo;
    vertex_attribute m_attributes;
    bool m_has_indices=false;
    uint32_t m_id=-1;
};