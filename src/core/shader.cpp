#include <core/shader.hpp>
#include <glad/glad.h>
#include <fstream>
#include <array>
#include <print>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

static int shader_to_stage(shader_stage p_stage) {
    switch (p_stage){
    case shader_stage::vertex:
        return GL_VERTEX_SHADER;
    case shader_stage::fragment:
        return GL_FRAGMENT_SHADER;
    default:
        break;
    }

    return 0;
}

shader::shader(const std::span<shader_info>& p_shader_sources) {
    invalidate(p_shader_sources);
    m_shader_loaded = true;
}

shader::~shader() {
    glUseProgram(0);
}

void shader::invalidate(const std::span<shader_info>& p_shader_sources) {
    m_shader_sources_ids.resize(p_shader_sources.size());
    
    m_shader_program = glCreateProgram();

    int success;


    for(size_t i = 0; i < p_shader_sources.size(); i++) {
        const shader_info& source = p_shader_sources[i];
        uint32_t shader_src_id = load_shader(source);
        if(shader_src_id != -1) {
            m_shader_sources_ids[i] = shader_src_id;
        }
    }

    glLinkProgram(m_shader_program);
    glGetProgramiv(m_shader_program, GL_LINK_STATUS, &success);
    if(!success){
        // char infoLog[512];
        std::array<char, 512> info_log;
        glGetProgramInfoLog(m_shader_program, info_log.size(), nullptr, info_log.data());
        std::print("Could NOT load shader program!\n");
        std::print("{}\n", info_log.data());
        m_shader_loaded = false;
        return;
    }

    // once we load and set the shader sources are compiled and linked we delete those shader ID's

    for(auto& shader_id : m_shader_sources_ids) {
        glDeleteShader(shader_id);
    }

    m_shader_loaded = true;
}

void shader::bind() {
    glUseProgram(m_shader_program);
}

void shader::unbind() {
    glUseProgram(0);
}

uint32_t shader::load_shader(const shader_info& p_info) {
    uint32_t shader_source_id = -1;
    std::ifstream ins(p_info.path);
    int success;
    std::array<char, 512> info_log;

    if(!ins){
        return false;
    }

    std::stringstream ss;
    ss << ins.rdbuf();
    // This will contain the actual shader literal src
    // const char* shader_src = ss.str().c_str();
    std::string src = ss.str();
    const char* shader_src = src.c_str();

    shader_source_id = glCreateShader(shader_to_stage(p_info.stage));
    glShaderSource(shader_source_id, 1, &shader_src, nullptr);
    glCompileShader(shader_source_id);
    glGetShaderiv(shader_source_id, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(shader_source_id, info_log.size(), nullptr, info_log.data());
        std::println("{}", info_log.data());
        return false;
    }

    glAttachShader(m_shader_program, shader_source_id);

    return shader_source_id;
}

int shader::location(const std::string& p_name){
    return glGetUniformLocation(m_shader_program, p_name.c_str());
}

void shader::write(const std::string& name, const int& p_value){
    glUniform1i(location(name), p_value);
}

void shader::write(const std::string& name, const float& p_value){
    glUniform1f(location(name), p_value);
}

void shader::write(const std::string& name, const glm::vec2& p_value){
    glUniform2f(location(name), p_value.x, p_value.y);
}

void shader::write(const std::string& name, const glm::vec3& p_value){
    glUniform3f(location(name), p_value.x, p_value.y, p_value.z);
}

void shader::write(const std::string& name, const glm::vec4& p_value){
    glUniform4f(location(name), p_value.x, p_value.y, p_value.z, p_value.w);
}

void shader::write(const std::string& name, const glm::mat2& p_value){
    glUniformMatrix2fv(location(name), 1, false, glm::value_ptr(p_value));
}

void shader::write(const std::string& name, const glm::mat3& p_value){
    glUniformMatrix3fv(location(name), 1, false, glm::value_ptr(p_value));
}

void shader::write(const std::string& name, const glm::mat4& p_value){
    glUniformMatrix4fv(location(name), 1, false, glm::value_ptr(p_value));
}