#pragma once
#include <span>

class index_buffer {
public:
    index_buffer() = default;
    index_buffer(const std::span<const uint32_t>& p_indices);

    void bind();
    void unbind();
    
    void invalidate(const std::span<const uint32_t>& p_indices);

    [[nodiscard]] uint32_t id() const { return m_id; }

    [[nodiscard]] uint32_t size() const { return m_indices_size; }

private:
    uint32_t m_id=-1;
    uint32_t m_indices_size=0;
};