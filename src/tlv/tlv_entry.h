#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

#include "graphics/mesh_data.h"
#include "graphics/texture.h"
#include "graphics/vertex_data.h"

using namespace std::literals;

namespace game
{

/**
 * Enumeration of TLV types.
 */
enum class TLVType : std::uint32_t
{
    UINT32,
    UINT32_ARRAY,
    STRING,
    BYTE_ARRAY,
    TEXTURE_FORMAT,
    TEXTURE_USAGE,
    VERTEX_DATA,
    VERTEX_DATA_ARRAY,

    // composite types
    TEXTURE_DESCRIPTION,
    MESH_DATA
};

/**
 * A Type-Length-Value entry. This is a non-owning view into a TLV entry.
 *
 * An entry is a variable length structure with the following layout:
 *
 * +--------+ -.
 * |  Type  |  | 4 bytes
 * +--------+ -+
 * | Length |  | 4 bytes
 * +--------+ -+
 * | Value  |  |
 * |        |  | Length bytes
 * |  ...   |  |
 * +--------+ -'
 *
 * It is assumed TLVs will contain user controlled data so they have wide contracts.
 */
class TLVEntry
{
  public:
    /**
     * Construct a new TLV entry.
     *
     * @param type
     *   The type of the entry.
     * @param value
     *   The data for the entry.
     */
    TLVEntry(TLVType type, std::span<const std::byte> value);

    /**
     * Get the type of the entry.
     *
     * @returns
     *   The type of the entry.
     */
    auto type() const -> TLVType;

    /**
     * Get a copy of the value as a uint32_t. Will throw if the type does not match.
     *
     * @returns
     *  The value of the entry as a uint32_t.
     */
    auto uint32_value() const -> std::uint32_t;

    /**
     * Get a copy of the value as a uint32_t array. Will throw if the type does not match.
     *
     * @returns
     *  The value of the entry as a uint32_t array.
     */
    auto uint32_array_value() const -> std::vector<std::uint32_t>;

    /**
     * Get a copy of the value as a string. Will throw if the type does not match.
     *
     * @returns
     *  The value of the entry as a string.
     */
    auto string_value() const -> std::string;

    /**
     * Get a copy of the value as a byte array. Will throw if the type does not match.
     *
     * @returns
     *  The value of the entry as a byte array.
     */
    auto byte_array_value() const -> std::vector<std::byte>;

    /**
     * Get a copy of the value as a texture format. Will throw if the type does not match.
     *
     * @returns
     *  The value of the entry as a texture format.
     */
    auto texture_format_value() const -> TextureFormat;

    /**
     * Get a copy of the value as a texture usage. Will throw if the type does not match.
     *
     * @returns
     *  The value of the entry as a texture usage.
     */
    auto texture_usage_value() const -> TextureUsage;

    /**
     * Get a copy of the value as a texture description. Will throw if the type does not match.
     *
     * @returns
     *  The value of the entry as a texture description.
     */
    auto texture_description_value() const -> TextureDescription;

    /**
     * Check if the entry is a texture with the given name.
     *
     * @param name
     *   The name of the texture.
     *
     * @returns
     *   True if the entry is a texture with the given name, false otherwise.
     */
    auto is_texture(std::string_view name) const -> bool;

    /**
     * Get a copy of the value as a vertex data. Will throw if the type does not match.
     *
     * @returns
     *  The value of the entry as a vertex data.
     */
    auto vertex_data_value() const -> VertexData;

    /**
     * Get a copy of the value as a vertex data array. Will throw if the type does not match.
     *
     * @returns
     *  The value of the entry as a vertex data array.
     */
    auto vertex_data_array_value() const -> std::vector<VertexData>;

    /**
     * Get a copy of the value as a mesh. Will throw if the type does not match.
     *
     * @returns
     *  The value of the entry as a mesh.
     */
    auto mesh_value() const -> MeshData;

    /**
     * Check if the entry is a mesh with the given name.
     *
     * @param name
     *   The name of the mesh.
     *
     * @returns
     *   True if the entry is a mesh with the given name, false otherwise.
     */
    auto is_mesh(std::string_view name) const -> bool;

    /**
     * Get the size of the whole entry, type + length + value.
     *
     * @returns
     *   The size of the entry in bytes.
     */
    auto size() const -> std::uint32_t;

  private:
    /** The type of the entry. */
    TLVType type_;

    /** View into the value of the entry. */
    std::span<const std::byte> value_;
};

/**
 * Format the TLV type as a string.
 *
 * @param obj
 *   The TLV type to format.
 *
 * @returns
 *   The TLV type as a string.
 */
auto to_string(const game::TLVType &obj) -> std::string;

}
