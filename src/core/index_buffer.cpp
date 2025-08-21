#include <core/index_buffer.hpp>
#include <glad/glad.h>

index_buffer::index_buffer(const std::span<uint32_t>& p_indices) {
    glGenBuffers(1, &m_id);

    write(p_indices);
}

void index_buffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void index_buffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void index_buffer::write(const std::span<uint32_t>& p_indices) {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (uint32_t)p_indices.size_bytes(), p_indices.data(), GL_STATIC_DRAW);
    unbind();
}