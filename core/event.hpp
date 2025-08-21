#pragma once
#include <core/keys.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


bool is_key_pressed(GLFWwindow* p_window, int p_key);

bool is_key_released(GLFWwindow* p_window, int p_key);

bool is_mouse_pressed(GLFWwindow* p_window, int p_mouse_code);
bool is_mouse_released(GLFWwindow* p_window, int p_mouse_code);

glm::vec2 cursor_position(GLFWwindow* p_window);

// specific for listening events
void poll_events();