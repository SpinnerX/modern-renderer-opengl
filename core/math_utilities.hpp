#pragma once
#include <glm/glm.hpp>

glm::quat to_quat(const glm::highp_vec3& p_values);

glm::quat to_quat(const glm::vec4& p_values);

glm::highp_vec4 from_quat(const glm::vec3& p_values);

glm::quat to_quat(const glm::vec3& p_values);

glm::quat to_quathp(const glm::highp_vec4& p_values);
