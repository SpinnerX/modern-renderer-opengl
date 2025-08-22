#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <span>
#include <string>
#include <cstdint>
#include <print>

struct vertex {
    static constexpr uint32_t max_bone_influence=4;
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec3 tangent;
    glm::vec3 bit_tangent;

    bool operator==(const vertex& other) const {
        return position == other.position and color == other.color and
                uv == other.uv and normal == other.normal;
    }
};

enum class data_type : uint8_t {
    // float1, float2, float3, float4,
    float1,
    mat3, mat4,
    // int1, int2, int3, int4,
    int1,
    bool1
};

struct vertex_attribute_element {
    std::string name="Undefined";
    GLenum type;
    uint32_t size=0; // size of the components, essentially sizeof(glm::vec3)
    uint32_t offset=0;
    bool is_normalized=false;
};

/*

* in shader representation
layout(location = 0) in vec3 aPos;

* vertex_attribute_element representation
* .size represents the components, in this case glm::vec3 = 3 floats. So size is 3.
* .type represent what is the type of the type each component are, in this case glm::vec3 contains all three floats. Which is why specified as GL_FLOAT
std::array<vertex_attribute_element, 1> attributes = {
    vertex_attribute_element{ .name = "aPos", .type = GL_FLOAT, .size=3 }
};

*/
class vertex_attribute {
public:
    vertex_attribute() = default;
    vertex_attribute(const std::span<vertex_attribute_element>& p_list){
        calculate_stride_offset(p_list);
    }

    [[nodiscard]] uint32_t stride() const { return m_stride; }

private:
    void calculate_stride_offset(const std::span<vertex_attribute_element>& p_attributes) {
        uint32_t offset = 0;
        m_stride = 0;

        for(auto& attribute : p_attributes) {
            attribute.offset = offset;
            offset += attribute.size;
            m_stride += attribute.size;
        }
    }

private:
    uint32_t m_stride=0;
};

class vertex_buffer {
public:
    vertex_buffer() = default;
    vertex_buffer(const std::span<vertex>& p_vertices);
    vertex_buffer(const std::span<float>& p_vertices);
    ~vertex_buffer();
    
    void bind();
    void unbind();

    [[nodiscard]] uint32_t id() const { return m_id; }

private:
    void write(const std::span<float>& p_vertices);
    void write(const std::span<vertex>& p_vertices);

private:
    uint32_t m_id=-1;
};