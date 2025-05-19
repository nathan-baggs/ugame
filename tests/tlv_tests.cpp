#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ranges>
#include <span>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "graphics/texture.h"
#include "tlv/tlv_entry.h"
#include "tlv/tlv_reader.h"
#include "tlv/tlv_writer.h"
#include "utils/error.h"
#include "utils/exception.h"

#pragma comment(lib, "opengl32.lib")

namespace
{
template <class... Args>
auto create_binary_vec(Args... args) -> std::vector<std::byte>
{
    return {std::byte(args)...};
}
}

TEST(tlv_entry, ctor)
{
    const auto entry = game::TLVEntry{game::TLVType::UINT32, {}};
    ASSERT_EQ(entry.type(), game::TLVType::UINT32);
}

TEST(tlv_entry, uint32_value_valid)
{
    const auto bytes = create_binary_vec(0xdd, 0xcc, 0xbb, 0xaa);
    const auto entry = game::TLVEntry{game::TLVType::UINT32, bytes};

    ASSERT_EQ(entry.uint32_value(), 0xaabbccdd);
}

TEST(tlv_entry, uint32_value_invalid_size)
{
    const auto bytes = create_binary_vec(0xdd, 0xcc, 0xbb);
    const auto entry = game::TLVEntry{game::TLVType::UINT32, bytes};

    ASSERT_THROW(entry.uint32_value(), game::Exception);
}

TEST(tlv_entry, string_value_valid)
{
    const auto bytes = create_binary_vec('h', 'i');
    const auto entry = game::TLVEntry{game::TLVType::STRING, bytes};

    ASSERT_EQ(entry.string_value(), "hi");
}

TEST(tlv_entry, string_value_invalid_type)
{
    const auto bytes = create_binary_vec('h', 'i');
    const auto entry = game::TLVEntry{game::TLVType::UINT32, bytes};

    ASSERT_THROW(entry.string_value(), game::Exception);
}

TEST(tlv_entry, byte_array_value_valid)
{
    const auto bytes = create_binary_vec(0xaa, 0xbb, 0xcc);
    const auto entry = game::TLVEntry{game::TLVType::BYTE_ARRAY, bytes};

    ASSERT_EQ(entry.byte_array_value(), bytes);
}

TEST(tlv_entry, byte_array_value_invalid_type)
{
    const auto bytes = create_binary_vec('h', 'i');
    const auto entry = game::TLVEntry{game::TLVType::UINT32, bytes};

    ASSERT_THROW(entry.byte_array_value(), game::Exception);
}

TEST(tlv_reader, begin)
{
    const auto bytes = create_binary_vec(
        0x00,
        0x00,
        0x00,
        0x00, // type
        0x04,
        0x00,
        0x00,
        0x00, // length
        0xdd,
        0xcc,
        0xbb,
        0xaa // value
    );

    const auto reader = game::TLVReader(bytes);
    const auto begin = reader.begin();

    ASSERT_EQ((*begin).type(), game::TLVType::UINT32);
    ASSERT_EQ((*begin).uint32_value(), 0xaabbccdd);
}

TEST(tlv_reader, range_loop)
{
    const auto bytes = create_binary_vec(
        0x00,
        0x00,
        0x00,
        0x00, // type
        0x04,
        0x00,
        0x00,
        0x00, // length
        0xdd,
        0xcc,
        0xbb,
        0xaa, // value
        0x02,
        0x00,
        0x00,
        0x00, // type
        0x02,
        0x00,
        0x00,
        0x00, // length
        'y',
        'o' // value
    );

    const auto reader = game::TLVReader(bytes);
    for (const auto &[index, entry] : reader | std::views::enumerate)
    {
        if (index == 0u)
        {
            ASSERT_EQ(entry.uint32_value(), 0xaabbccdd);
        }
        else if (index == 1u)
        {
            ASSERT_EQ(entry.string_value(), "yo");
        }
        else
        {
            ASSERT_TRUE(false);
        }
    }
}

TEST(tlv_writer, write_uint32)
{
    auto value = std::uint32_t{0xaabbccdd};
    auto writer = game::TLVWriter{};

    writer.write(value);

    const auto buffer = writer.yield();
    auto reader = game::TLVReader{buffer};
    const auto entry = std::ranges::begin(reader);

    ASSERT_EQ(value, (*entry).uint32_value());
}

TEST(tlv_writer, write_string)
{
    auto value = std::string{"hello world"};
    auto writer = game::TLVWriter{};

    writer.write(value);

    const auto buffer = writer.yield();
    auto reader = game::TLVReader{buffer};
    const auto entry = std::ranges::begin(reader);

    ASSERT_EQ(value, (*entry).string_value());
}

TEST(tlv_writer, write_byte_array)
{
    const auto value = create_binary_vec(0xaa, 0xbb, 0xcc);
    auto writer = game::TLVWriter{};

    writer.write(value);

    const auto buffer = writer.yield();
    auto reader = game::TLVReader{buffer};
    const auto entry = std::ranges::begin(reader);

    ASSERT_EQ(value, (*entry).byte_array_value());
}

TEST(tlv_writer, write_texture_description)
{
    const auto name = std::string{"tex.png"};
    const auto width = std::uint32_t{100u};
    const auto height = std::uint32_t{101u};
    const auto format = game::TextureFormat::RGB;
    const auto usage = game::TextureUsage::SRGB;
    const auto data = create_binary_vec(0xaa, 0xbb, 0xcc);
    auto writer = game::TLVWriter{};

    writer.write(name, width, height, format, usage, data);

    const auto buffer = writer.yield();
    auto reader = game::TLVReader{buffer};
    const auto entry = std::ranges::begin(reader);
    const auto texture_desc = (*entry).texture_description_value();

    ASSERT_EQ(width, texture_desc.width);
    ASSERT_EQ(height, texture_desc.height);
    ASSERT_EQ(format, texture_desc.format);
    ASSERT_EQ(usage, texture_desc.usage);
    ASSERT_EQ(data, texture_desc.data);
}
