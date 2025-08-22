#include <core/vertex_array.hpp>
#include <print>

vertex_array::vertex_array(const std::span<float>& p_vertices, const std::span<uint32_t>& p_indices) {
    glGenVertexArrays(1, &m_id);
    glBindVertexArray(m_id);
    m_vbo = vertex_buffer(p_vertices);
    m_ibo = index_buffer(p_indices);

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
        std::println("Attribute size = {}", element.size);
        std::println("Stride = {}", m_attributes.stride());
        std::println("IsNormalized = {}\n", (element.is_normalized ? "true" : "false"));
        if(element.type == GL_FLOAT) {
            glVertexAttribPointer(index,
                        (int)element.size,
                        element.type,
                            element.is_normalized ? GL_TRUE : GL_FALSE,
                            int(m_attributes.stride() * sizeof(float)),
                            (const void*)(element.offset * sizeof(float))
            );
            glEnableVertexAttribArray(index);
            index++;
        }
    }

}