#pragma once
#include <filesystem>

enum class texture_spec{
    default_value,
    depth_map,
    normal_map,
    gamma_correction
};

struct image_extent {
    uint32_t width=-1;
    uint32_t height=-1;
};

class texture {
public:
    texture() = default;
    texture(const std::filesystem::path& p_path, bool p_gamma_enable=false);

    void bind(uint32_t p_index=0);

    void unbind();

    [[nodiscard]] bool loaded() const { return m_texture_loaded; }

    [[nodiscard]] uint32_t id() const { return m_texture_id; }

    void invalidate(const std::filesystem::path& p_path, bool p_gamma_enable=false);

private:
    uint32_t m_texture_id=-1;
    bool m_texture_loaded = false;
};