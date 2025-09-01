#pragma once
#include <filesystem>
#include <core/vertex_array.hpp>
#include <span>

class obj_loader {
public:
    obj_loader() = default;
    obj_loader(const std::filesystem::path& p_path);

    [[nodiscard]] bool loaded() const { return m_model_loaded; }
    [[nodiscard]] std::span<const vertex> vertices() const {
        return std::span<const vertex>(m_vertices.begin(),m_vertices.end());
    }

    [[nodiscard]] std::span<const uint32_t> indices() const {
        return m_indices;
    }

private:
    std::vector<vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    bool m_model_loaded=false;
};