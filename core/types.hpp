#pragma once
#include <cstdint>

enum class attribute_type : uint8_t {
    none=0,
    float1, float2, float3, float4,
    mat3, mat4,
    int1, int2, int3, int4,
    uint1, uint32,uint64,
    bool1
};