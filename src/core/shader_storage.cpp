#include <core/shader_storage.hpp>

void shader_storage::load_shader(shader_type p_type, const std::span<shader_info>& p_shader_src) {
    m_shader_cached[p_type] = std::make_shared<shader>(p_shader_src);
}