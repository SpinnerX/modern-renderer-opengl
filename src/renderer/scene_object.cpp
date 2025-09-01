#include <renderer/scene_object.hpp>

scene_object::scene_object(const std::string& p_name, flecs::world* p_world) {
    m_entity = p_world->entity(p_name.c_str());
}