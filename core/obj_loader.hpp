#pragma once
#include <filesystem>
// #include <core/vertex_buffer.hpp>
// #include <core/index_buffer.hpp>
#include <core/vertex_array.hpp>

class obj_loader {
public:
    obj_loader() = default;
    obj_loader(const std::filesystem::path& p_path);

    [[nodiscard]] bool loaded() const;

    void bind();
    void unbind();

    void draw();

private:
    // vertex_buffer m_vbo;
    // index_buffer m_ibo;
    vertex_array m_vao;
    bool m_model_loaded=false;
};