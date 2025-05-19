#pragma once

#include <cstdint>
#include <span>

#include "graphics/vertex_data.h"

namespace game
{
struct MeshData
{
    std::span<const VertexData> vertices;
    std::span<const std::uint32_t> indices;
};

}
