#include <core/index_buffer.hpp>
#include <glad/glad.h>
#include <print>

index_buffer::index_buffer(const std::span<const uint32_t>& p_indices) {
    glGenBuffers(1, &m_id);

    invalidate(p_indices);
    m_indices_size = p_indices.size();

    std::println("index_buffer constructed!!!");
}

void index_buffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void index_buffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void index_buffer::invalidate(const std::span<const uint32_t>& p_indices) {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)p_indices.size_bytes(), p_indices.data(), GL_STATIC_DRAW);
}