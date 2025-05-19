#include "graphics/mesh_factory.h"

#include <cstdint>
#include <ranges>
#include <vector>

#include "graphics/vertex_data.h"
#include "maths/vector3.h"

namespace
{
/**
 * Helper function to create a vector of VertexData from a variadic list of arguments.
 *
 * @param args
 *   The arguments to be used to create VertexData objects.
 *
 * @returns
 *   A vector of VertexData objects created from the provided arguments.
 */
template <class... Args>
std::vector<game::VertexData> vertices(Args &&...args)
{
    return std::views::zip_transform(
               []<class... A>(A &&...a) { return game::VertexData{std::forward<A>(a)...}; },
               std::forward<Args>(args)...) |
           std::ranges::to<std::vector>();
}
}

namespace game
{

auto MeshFactory::cube() -> MeshData
{
    const auto loaded = loaded_meshes_.find("cube");
    if (loaded != std::ranges::cend(loaded_meshes_))
    {
        return {.vertices = loaded->second.vertices, .indices = loaded->second.indices};
    }

    const Vector3 positions[] = {{-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
                                 {-1.0f, 1.0f, 1.0f},   {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},
                                 {1.0f, 1.0f, -1.0f},   {-1.0f, 1.0f, -1.0f},  {-1.0f, -1.0f, -1.0f},
                                 {-1.0f, -1.0f, 1.0f},  {-1.0f, 1.0f, 1.0f},   {-1.0f, 1.0f, -1.0f},
                                 {1.0f, -1.0f, -1.0f},  {1.0f, -1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
                                 {1.0f, 1.0f, -1.0f},   {-1.0f, 1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
                                 {1.0f, 1.0f, -1.0f},   {-1.0f, 1.0f, -1.0f},  {-1.0f, -1.0f, 1.0f},
                                 {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},  {1.0f, -1.0f, 1.0f}};

    const Vector3 normals[] = {{0.0f, 0.0f, 1.0f},  {0.0f, 0.0f, 1.0f},  {0.0f, 0.0f, 1.0f},  {0.0f, 0.0f, 1.0f},
                               {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
                               {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
                               {1.0f, 0.0f, 0.0f},  {1.0f, 0.0f, 0.0f},  {1.0f, 0.0f, 0.0f},  {1.0f, 0.0f, 0.0f},
                               {0.0f, 1.0f, 0.0f},  {0.0f, 1.0f, 0.0f},  {0.0f, 1.0f, 0.0f},  {0.0f, 1.0f, 0.0f},
                               {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}};

    const UV uvs[] = {
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
    };

    auto indices = std::vector<std::uint32_t>{
        0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
        12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
    };

    const auto new_item =
        loaded_meshes_.emplace("cube", LoadedMeshData{vertices(positions, normals, normals, uvs), std::move(indices)});

    return {.vertices = new_item.first->second.vertices, .indices = new_item.first->second.indices};
}

auto MeshFactory::sprite() -> MeshData
{
    const auto loaded = loaded_meshes_.find("sprite");
    if (loaded != std::ranges::cend(loaded_meshes_))
    {
        return {.vertices = loaded->second.vertices, .indices = loaded->second.indices};
    }

    const Vector3 positions[] = {{-1.0f, 1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}};

    const UV uvs[] = {{0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}};

    auto indices = std::vector<std::uint32_t>{0, 1, 2, 0, 2, 3};

    const auto new_item = loaded_meshes_.emplace(
        "sprite", LoadedMeshData{vertices(positions, positions, positions, uvs), std::move(indices)});

    return {.vertices = new_item.first->second.vertices, .indices = new_item.first->second.indices};
}

};
