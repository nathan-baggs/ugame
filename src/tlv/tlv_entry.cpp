#include "tlv/tlv_entry.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <string>
#include <vector>

#include "graphics/mesh_data.h"
#include "graphics/texture.h"
#include "graphics/vertex_data.h"
#include "tlv/tlv_reader.h"
#include "utils/error.h"

namespace game
{

TLVEntry::TLVEntry(TLVType type, std::span<const std::byte> value)
    : type_(type)
    , value_(value)
{
}

auto TLVEntry::type() const -> TLVType
{
    return type_;
}

auto TLVEntry::uint32_value() const -> std::uint32_t
{
    ensure(type_ == TLVType::UINT32, "incorrect type");
    ensure(value_.size() == sizeof(std::uint32_t), "incorrect size");

    auto value = std::uint32_t{};
    std::memcpy(&value, value_.data(), sizeof(value));
    return value;
}

auto TLVEntry::uint32_array_value() const -> std::vector<std::uint32_t>
{
    ensure(type_ == TLVType::UINT32_ARRAY, "incorrect type");

    auto value = std::vector<std::uint32_t>(value_.size() / sizeof(std::uint32_t));
    std::memcpy(value.data(), value_.data(), value_.size());

    return value;
}

auto TLVEntry::string_value() const -> std::string
{
    ensure(type_ == TLVType::STRING, "incorrect type");

    const auto *ptr = reinterpret_cast<const char *>(value_.data());
    return std::string(ptr, ptr + value_.size());
}

auto TLVEntry::byte_array_value() const -> std::vector<std::byte>
{
    ensure(type_ == TLVType::BYTE_ARRAY, "incorrect type");

    const auto *ptr = reinterpret_cast<const std::byte *>(value_.data());
    return std::vector<std::byte>(ptr, ptr + value_.size());
}

auto TLVEntry::size() const -> std::uint32_t
{
    return static_cast<std::uint32_t>(sizeof(type_) + sizeof(std::uint32_t) + value_.size());
}

auto TLVEntry::texture_format_value() const -> TextureFormat
{
    ensure(type_ == TLVType::TEXTURE_FORMAT, "incorrect type");
    ensure(value_.size() == sizeof(TextureFormat), "incorrect size");

    auto value = TextureFormat{};
    std::memcpy(&value, value_.data(), sizeof(value));
    return value;
}

auto TLVEntry::texture_usage_value() const -> TextureUsage
{
    ensure(type_ == TLVType::TEXTURE_USAGE, "incorrect type");
    ensure(value_.size() == sizeof(TextureUsage), "incorrect size");

    auto value = TextureUsage{};
    std::memcpy(&value, value_.data(), sizeof(value));
    return value;
}

auto TLVEntry::texture_description_value() const -> TextureDescription
{
    ensure(type_ == TLVType::TEXTURE_DESCRIPTION, "incorrect type");

    const auto reader = TLVReader(value_);
    auto reader_cursor = std::ranges::begin(reader);

    ensure((*reader_cursor).type() == TLVType::STRING, "first member not string");
    ++reader_cursor;

    const auto width = (*reader_cursor).uint32_value();
    ++reader_cursor;
    ensure(reader_cursor != std::ranges::end(reader), "texture TLV too small");

    const auto height = (*reader_cursor).uint32_value();
    ++reader_cursor;
    ensure(reader_cursor != std::ranges::end(reader), "texture TLV too small");

    [[maybe_unused]] const auto format = (*reader_cursor).texture_format_value();
    ++reader_cursor;
    ensure(reader_cursor != std::ranges::end(reader), "texture TLV too small");

    const auto usage = (*reader_cursor).texture_usage_value();
    ++reader_cursor;
    ensure(reader_cursor != std::ranges::end(reader), "texture TLV too small");

    auto data = (*reader_cursor).byte_array_value();
    ++reader_cursor;
    ensure(reader_cursor == std::ranges::end(reader), "texture TLV too large");

    return {width, height, format, usage, std::move(data)};
}

auto TLVEntry::is_texture(std::string_view name) const -> bool
{
    if (type_ != TLVType::TEXTURE_DESCRIPTION)
    {
        return false;
    }

    const auto reader = TLVReader(value_);
    auto reader_cursor = std::ranges::begin(reader);

    const auto texture_name = (*reader_cursor).string_value();
    return texture_name == name;
}

auto TLVEntry::vertex_data_value() const -> VertexData
{
    ensure(type_ == TLVType::VERTEX_DATA, "incorrect type");
    ensure(value_.size() == sizeof(VertexData), "incorrect size");

    auto value = VertexData{};
    std::memcpy(&value, value_.data(), sizeof(value));
    return value;
}

auto TLVEntry::vertex_data_array_value() const -> std::vector<VertexData>
{
    ensure(type_ == TLVType::VERTEX_DATA_ARRAY, "incorrect type");

    auto value = std::vector<VertexData>(value_.size() / sizeof(VertexData));
    std::memcpy(value.data(), value_.data(), value_.size());

    return value;
}

auto TLVEntry::mesh_value() const -> MeshData
{
    ensure(type_ == TLVType::MESH_DATA, "incorrect type");

    const auto reader = TLVReader(value_);
    auto reader_cursor = std::ranges::begin(reader);

    ensure((*reader_cursor).type() == TLVType::STRING, "first member not string");
    ++reader_cursor;

    ensure((*reader_cursor).type() == TLVType::VERTEX_DATA_ARRAY, "second member not vertex data array");
    const auto vertex_data = std::span<const VertexData>{
        reinterpret_cast<const VertexData *>((*reader_cursor).value_.data()),
        (*reader_cursor).value_.size() / sizeof(VertexData)};
    ++reader_cursor;

    ensure((*reader_cursor).type() == TLVType::UINT32_ARRAY, "third member not uint32 array");
    const auto index_data = std::span<const std::uint32_t>{
        reinterpret_cast<const std::uint32_t *>((*reader_cursor).value_.data()),
        (*reader_cursor).value_.size() / sizeof(std::uint32_t)};

    ++reader_cursor;
    ensure(reader_cursor == std::ranges::end(reader), "texture TLV too large");

    return {vertex_data, index_data};
}

auto TLVEntry::is_mesh(std::string_view name) const -> bool
{
    if (type_ != TLVType::MESH_DATA)
    {
        return false;
    }

    const auto reader = TLVReader(value_);
    auto reader_cursor = std::ranges::begin(reader);

    const auto mesh_name = (*reader_cursor).string_value();
    return mesh_name == name;
}

auto to_string(const game::TLVType &obj) -> std::string
{
    auto str = "unknown"sv;
    switch (obj)
    {
        using enum game::TLVType;

        case UINT32: str = "UINT32"sv; break;
        case UINT32_ARRAY: str = "UINT32_ARRAY"sv; break;
        case STRING: str = "STRING"sv; break;
        case BYTE_ARRAY: str = "BYTE_ARRAY"sv; break;
        case TEXTURE_FORMAT: str = "TEXTURE_FORMAT"sv; break;
        case TEXTURE_USAGE: str = "TEXTURE_USAGE"sv; break;
        case VERTEX_DATA: str = "VERTEX_DATA"sv; break;
        case VERTEX_DATA_ARRAY: str = "VERTEX_DATA_ARRAY"sv; break;

        case TEXTURE_DESCRIPTION: str = "TEXTURE_DESCRIPTION"sv; break;
        case MESH_DATA: str = "MESH_DATA"sv; break;
    }

    return std::format("{}", str);
}

}
