#include "tlv/tlv_writer.h"

#include <cstddef>
#include <cstdint>
#include <ranges>
#include <span>
#include <string_view>
#include <vector>

#include "graphics/vertex_data.h"
#include "tlv/tlv_entry.h"

namespace
{

auto write_bytes(std::vector<std::byte> &buffer, std::span<const std::byte> data) -> void
{
    buffer.insert(std::ranges::end(buffer), std::ranges::cbegin(data), std::ranges::cend(data));
}

auto write_entry(
    std::vector<std::byte> &buffer,
    game::TLVType type,
    std::uint32_t length,
    std::span<const std::byte> value) -> void
{
    write_bytes(buffer, {reinterpret_cast<const std::byte *>(&type), sizeof(type)});
    write_bytes(buffer, {reinterpret_cast<const std::byte *>(&length), sizeof(length)});
    write_bytes(buffer, value);
}

}

namespace game
{

auto TLVWriter::yield() -> std::vector<std::byte>
{
    auto tmp = std::vector<std::byte>{};
    std::ranges::swap(tmp, buffer_);

    return tmp;
}

auto TLVWriter::write(std::uint32_t value) -> void
{
    const auto type = TLVType::UINT32;
    const auto length = sizeof(value);
    const auto value_bytes = std::span<const std::byte>{reinterpret_cast<const std::byte *>(&value), length};
    write_entry(buffer_, type, length, value_bytes);
}

auto TLVWriter::write(std::span<const std::uint32_t> value) -> void
{
    const auto type = TLVType::UINT32_ARRAY;
    const auto length = static_cast<std::uint32_t>(value.size() * sizeof(std::uint32_t));
    const auto value_bytes = std::span<const std::byte>{reinterpret_cast<const std::byte *>(value.data()), length};
    write_entry(buffer_, type, length, value_bytes);
}

auto TLVWriter::write(std::string_view value) -> void
{
    const auto type = TLVType::STRING;
    const auto length = static_cast<std::uint32_t>(value.length());
    const auto value_bytes = std::span<const std::byte>{reinterpret_cast<const std::byte *>(value.data()), length};
    write_entry(buffer_, type, length, value_bytes);
}

auto TLVWriter::write(std::span<const std::byte> value) -> void
{
    const auto type = TLVType::BYTE_ARRAY;
    const auto length = static_cast<std::uint32_t>(value.size());
    const auto value_bytes = std::span<const std::byte>{reinterpret_cast<const std::byte *>(value.data()), length};
    write_entry(buffer_, type, length, value_bytes);
}

auto TLVWriter::write(TextureFormat value) -> void
{
    const auto type = TLVType::TEXTURE_FORMAT;
    const auto length = sizeof(value);
    const auto value_bytes = std::span<const std::byte>{reinterpret_cast<const std::byte *>(&value), length};
    write_entry(buffer_, type, length, value_bytes);
}

auto TLVWriter::write(TextureUsage value) -> void
{
    const auto type = TLVType::TEXTURE_USAGE;
    const auto length = sizeof(value);
    const auto value_bytes = std::span<const std::byte>{reinterpret_cast<const std::byte *>(&value), length};
    write_entry(buffer_, type, length, value_bytes);
}

auto TLVWriter::write(const VertexData &value) -> void
{
    const auto type = TLVType::VERTEX_DATA;
    const auto length = sizeof(value);
    const auto value_bytes = std::span<const std::byte>{reinterpret_cast<const std::byte *>(&value), length};
    write_entry(buffer_, type, length, value_bytes);
}

auto TLVWriter::write(std::span<const VertexData> value) -> void
{
    const auto type = TLVType::VERTEX_DATA_ARRAY;
    const auto length = static_cast<std::uint32_t>(value.size() * sizeof(VertexData));
    const auto value_bytes = std::span<const std::byte>{reinterpret_cast<const std::byte *>(value.data()), length};
    write_entry(buffer_, type, length, value_bytes);
}

auto TLVWriter::write(
    std::string_view name,
    std::uint32_t width,
    std::uint32_t height,
    TextureFormat format,
    TextureUsage usage,
    std::span<const std::byte> data) -> void
{
    auto writer = TLVWriter{};

    writer.write(name);
    writer.write(width);
    writer.write(height);
    writer.write(format);
    writer.write(usage);
    writer.write(data);

    const auto value = writer.yield();
    const auto type = TLVType::TEXTURE_DESCRIPTION;
    const auto length = static_cast<std::uint32_t>(value.size());
    write_entry(buffer_, type, length, value);
}

auto TLVWriter::write(
    std::string_view name,
    std::span<const VertexData> vertices,
    std::span<const std::uint32_t> indices) -> void
{
    auto writer = TLVWriter{};

    writer.write(name);
    writer.write(vertices);
    writer.write(indices);

    const auto value = writer.yield();
    const auto type = TLVType::MESH_DATA;
    const auto length = static_cast<std::uint32_t>(value.size());
    write_entry(buffer_, type, length, value);
}

}
