#pragma once

#include <span>
#include <vector>

#include "maths/matrix4.h"
#include "maths/quaternion.h"
#include "maths/transform.h"
#include "maths/vector3.h"

namespace game
{

class Material;
class Mesh;
class Texture;
class Sampler;

/**
 * Encapsulates all the state needed to render a single object in the world.
 *
 * Note that this class stores non-owning pointers to its properties. This means that the properties must outlive this
 * object. This is not enforced by the class, so it is up to the user to ensure that this is the case.
 */
class Entity
{
  public:
    /**
     * Construct a new Entity object.
     *
     * @param mesh
     *   The mesh to use for this entity.
     * @param material
     *   The material to use for this entity.
     * @param position
     *   The position of the entity in world space.
     * @param scale
     *   The scale of the entity in world space.
     * @param textures
     *   The textures to use for this entity.
     */
    Entity(
        const Mesh *mesh,
        const Material *material,
        const Vector3 &position,
        const Vector3 &scale,
        std::span<const Texture *const> textures);

    /**
     * Get the mesh for this entity.
     *
     * @returns
     *  The mesh for this entity.
     */
    auto mesh() const -> const Mesh *;

    /**
     * Get the material for this entity.
     *
     * @returns
     *   The material for this entity.
     */
    auto material() const -> const Material *;

    /**
     * Get the model matrix for this entity.
     *
     * @returns
     *   The model matrix for this entity.
     */
    auto model() const -> const Matrix4 &;

    /**
     * Get the textures for this entity.
     *
     * @returns
     *   The textures for this entity.
     */
    auto textures() const -> std::span<const Texture *const>;

    /**
     * Set the position of this entity.
     *
     * @param position
     *   The position to set.
     */
    auto set_position(const Vector3 &position) -> void;

    /**
     * Set the rotation of this entity.
     *
     * @param rotation
     *   The rotation to set.
     */
    auto set_rotation(const Quaternion &rotation) -> void;

    /**
     * Translate the entity by the given vector.
     *
     * @param translation
     *   The translation vector.
     */
    auto translate(const Vector3 &translation) -> void;

    /**
     * Get the transform for this entity.
     *
     * @returns
     *   The transform for this entity.
     */
    auto transform() const -> const Transform &;

    /**
     * Get the position of this entity.
     *
     * @returns
     *   The position of this entity.
     */
    auto position() const -> Vector3;

  private:
    /** Mesh for this entity. */
    const Mesh *mesh_;

    /** Material for this entity. */
    const Material *material_;

    /** Transform for this entity. */
    Transform transform_;

    /** Textures for this entity. */
    std::vector<const Texture *> textures_;
};

}
