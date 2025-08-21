#pragma once
#include <span>

class index_buffer {
public:
    index_buffer() = default;
    index_buffer(const std::span<uint32_t>& p_indices);

    void bind();
    void unbind();
    
    void write(const std::span<uint32_t>& p_indices);

    [[nodiscard]] uint32_t id() const { return m_id; }

private:
    uint32_t m_id=-1;
};