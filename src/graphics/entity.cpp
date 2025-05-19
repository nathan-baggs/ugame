#include "entity.h"

#include <span>
#include <vector>

#include "graphics/material.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "maths/vector3.h"

namespace game
{

Entity::Entity(
    const Mesh *mesh,
    const Material *material,
    const Vector3 &position,
    const Vector3 &scale,
    std::span<const Texture *const> textures)
    : mesh_(mesh)
    , material_(material)
    , transform_(position, scale, {})
    , textures_(std::ranges::cbegin(textures), std::ranges::cend(textures))
{
}

auto Entity::mesh() const -> const Mesh *
{
    return mesh_;
}

auto Entity::material() const -> const Material *
{
    return material_;
}

auto Entity::transform() const -> const Transform &
{
    return transform_;
}

auto Entity::textures() const -> std::span<const Texture *const>
{
    return textures_;
}

auto Entity::set_position(const Vector3 &position) -> void
{
    transform_.position = position;
}

auto Entity::set_rotation(const Quaternion &rotation) -> void
{
    transform_.rotation = rotation;
}

auto Entity::translate(const Vector3 &translation) -> void
{
    transform_.position += translation;
}

auto Entity::position() const -> Vector3
{
    return transform_.position;
}

}
