#pragma once
#include <string>
#include <flecs.h>

/**
 * @brief represents high-level of an object contained within a scene
*/
class scene_object {
public:
    scene_object(const std::string& p_name, flecs::world* p_world);

    template<typename UComponent>
    void add() {
        m_entity.add<UComponent>();
    }

    template<typename UComponent>
    void remove() {
        m_entity.remove<UComponent>();
    }

    template<typename UComponent>
    bool has() {
        return m_entity.has<UComponent>();
    }

private:
    flecs::entity m_entity;
};