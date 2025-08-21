#pragma once
#include <string>
#include <span>
#include <vector>
#include <glm/glm.hpp>

enum class shader_stage : uint8_t {
    vertex,
    fragment,
    compute
};

struct shader_info {
    std::string path;
    shader_stage stage;
};

class shader {
public:
    shader() = default;
    shader(const std::span<shader_info>& p_shader_sources);
    ~shader();

    void bind();

    void unbind();

    void invalidate(const std::span<shader_info>& p_shader_sources);

    [[nodiscard]] bool loaded() const { return m_shader_loaded; }

    void write(const std::string& name, const int& p_value);

    void write(const std::string& name, const float& p_value);
    void write(const std::string& name, const glm::vec2& p_value);

    void write(const std::string& name, const glm::vec3& p_value);


    void write(const std::string& name, const glm::vec4& p_value);

    void write(const std::string& name, const glm::mat2& p_value);

    void write(const std::string& name, const glm::mat3& p_value);

    void write(const std::string& name, const glm::mat4& p_value);

private:
    int location(const std::string& p_name);

private:
    uint32_t load_shader(const shader_info& p_info);
private:
    bool m_shader_loaded=false;

    uint32_t m_shader_program=-1;

    std::vector<uint32_t> m_shader_sources_ids;
};