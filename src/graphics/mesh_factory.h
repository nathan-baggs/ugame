#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include "graphics/mesh_data.h"
#include "graphics/vertex_data.h"
#include "utils/string_map.h"

namespace game
{

/**
 * Factory class for creating common mesh data.
 */
class MeshFactory
{
  public:
    /**
     * Create a cube mesh.
     */
    auto cube() -> MeshData;

    /**
     * Create a sprite mesh.
     */
    auto sprite() -> MeshData;

  private:
    /**
     * Struct to hold loaded mesh data.
     */
    struct LoadedMeshData
    {
        /** Vertex data for the mesh. */
        std::vector<VertexData> vertices;

        /** Index data for the mesh. */
        std::vector<std::uint32_t> indices;
    };

    /** Cache of loaded meshes. */
    StringMap<LoadedMeshData> loaded_meshes_;
};

}
