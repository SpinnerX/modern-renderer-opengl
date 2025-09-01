#pragma once
#include <filesystem>
#include <core/shader.hpp>
#include <map>

/**
 * @brief storage of all of the shaders loaded into the renderer
 * TODO: Could also potentially store custom shaders...
*/
class shader_storage {
public:
    shader_storage() = default;

    void add(const std::string& p_name, const std::span<const shader_info>& p_shader_sources);

    shader* get(const std::string& p_name) {
        return &m_shader_storage[p_name];
    }

private:
    // Probably store this as an enum to ensure we are retrieving a valid cached shader thats been stored
    std::map<std::string, shader> m_shader_storage;
};