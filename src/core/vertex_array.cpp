#include <core/vertex_array.hpp>
#include <print>

vertex_array::vertex_array(const std::span<float>& p_vertices) {
    std::println("vertex array constructed!!!");
    glGenVertexArrays(1, &m_id);
    bind();
    m_vbo = vertex_buffer(p_vertices);
    m_has_indices = false;
    std::println("vertex array constructed!!!");
}

vertex_array::vertex_array(const std::span<vertex>& p_vertices) {
    std::println("vertex array constructed!!!");
    glGenVertexArrays(1, &m_id);
    bind();
    m_vbo = vertex_buffer(p_vertices);
    std::println("vertex array constructed!!!");
    m_has_indices = false;
}

vertex_array::vertex_array(const std::span<uint32_t>& p_indices, const std::span<vertex>& p_vertices) {
    glGenVertexArrays(1, &m_id);
    bind();
    m_vbo = vertex_buffer(p_vertices);
    unbind();

    bind();
    m_ibo = index_buffer(p_indices);
    unbind();
    m_has_indices = true;
}

void vertex_array::bind() {
    glBindVertexArray(m_id);
}

void vertex_array::unbind() {
    glBindVertexArray(0);
}

void vertex_array::vertex_attributes(const std::span<vertex_attribute_element>& p_attribute_elements) {
    m_attributes = vertex_attribute(p_attribute_elements);
    uint32_t index = 0;
    bind();
    m_vbo.bind();

    for(auto element : p_attribute_elements) {
        std::println("Index = {}", index);
        std::println("Attribute offset/Size = {}", element.offset);
        std::println("Stride = {}", m_attributes.stride());
        std::println("IsNormalized = {}\n", (element.is_normalized ? "true" : "false"));
        int stride = static_cast<int>(m_attributes.stride() * sizeof(float));
        if(element.type == GL_FLOAT) {
            int offset = static_cast<int>(element.offset * sizeof(float));
            glVertexAttribPointer(index,
                        (int)element.size,
                        element.type,
                            element.is_normalized ? GL_TRUE : GL_FALSE,
                            stride,
                            (const void*)offset
            );
            glEnableVertexAttribArray(index);
            index++;
        }
    }

    m_vbo.unbind();
    unbind();
}