#pragma once
#include <core/shader.hpp>
#include <unordered_map>
#include <memory>

enum shader_type {
    geometry,
    lighting,
    shadows,
};

template<typename T>
using ref = std::shared_ptr<T>;

class shader_storage {
public:
    shader_storage() = default;

    void load_shader(shader_type p_type, const std::span<shader_info>& p_shader_src);

    ref<shader> get(shader_type p_type) {
        if(!m_shader_cached.contains(p_type)) {
            return nullptr;
        }

        return m_shader_cached[p_type];
    }

private:
    std::unordered_map<shader_type, ref<shader>> m_shader_cached;
};