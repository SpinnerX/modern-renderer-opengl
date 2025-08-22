#pragma once
#include <core/vertex_buffer.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

template<typename T, typename... Rest>
void hash_combine(size_t& seed, const T& v, const Rest&... rest) {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed << 2);
    (hash_combine(seed, rest), ...);
}

namespace std {

    template<>
    struct hash<vertex> {
        size_t operator()(const vertex& p_vertex) const {
            size_t seed = 0;
            hash_combine(
              seed, p_vertex.position, p_vertex.color, p_vertex.normal, p_vertex.uv);
            return seed;
        }
    };
}