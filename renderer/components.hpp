#pragma once
#include <glm/glm.hpp>
#include <core/math_utilities.hpp>
#include <filesystem>

struct transform {
    glm::highp_vec3 position{ 0.f };
    glm::highp_vec4 quaternion{ 0.f, 0, 0, 1 };
    glm::highp_vec3 rotation{ 0.f };
    glm::highp_vec3 scale{ 1.f };

    // Setting the rotation and automatically applies the quaternion after rotation's set
    void set_rotation(const glm::highp_vec3& p_rotation) {
        rotation = p_rotation;
        quaternion = from_quat(rotation);
    }
};

struct mesh_renderer {
    std::string model_path;
    glm::highp_vec4 color{1.f};
    std::vector<std::string> textures_path; // TODO: Have a material cache mechanism, eventually
};

/**
 * @brief because every object contains a transform component
 * 
 * This component will assume that is already handled and will only contain properties for configuring the point light itself.
*/
struct point_light {
    glm::vec3 direction;
};

struct perspective_camera {
    // glm::vec2 represented as {near: x, far: y}
    glm::vec2 plane{ 0.f };

    // Sets camera to be the main camera
    bool is_active = false;

    // Specify camera field of view
    // Defaults to 45.0f in radians
    float field_of_view = glm::radians(45.f);

};

struct projection_view {
    glm::mat4 projection;
    glm::mat4 view;
};

namespace tags {
    struct editor {};
};