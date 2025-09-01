#include <core/shader_storage.hpp>

void shader_storage::add(const std::string& p_name, const std::span<const shader_info>& p_shader_sources) {
    m_shader_storage.emplace(p_name, p_shader_sources);
}