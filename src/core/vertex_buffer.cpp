#include <core/vertex_buffer.hpp>
#include <glad/glad.h>

vertex_buffer::vertex_buffer(const std::span<vertex>& p_vertices) {
    glGenBuffers(1, &m_id);
    write(p_vertices);
}

vertex_buffer::vertex_buffer(const std::span<float>& p_vertices) {
    glGenBuffers(1, &m_id);
    write(p_vertices);
}

vertex_buffer::~vertex_buffer() {
    unbind();
}

void vertex_buffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void vertex_buffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vertex_buffer::write(const std::span<float>& p_vertices) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, (uint32_t)p_vertices.size_bytes(), p_vertices.data(), GL_STATIC_DRAW);
    unbind();
}

void vertex_buffer::write(const std::span<vertex>& p_vertices) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, (uint32_t)p_vertices.size_bytes(), p_vertices.data(), GL_STATIC_DRAW);
    unbind();
}