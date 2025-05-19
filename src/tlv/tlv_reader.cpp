#include "tlv/tlv_reader.h"

#include <cstdint>
#include <span>

#include "tlv/tlv_entry.h"
#include "utils/error.h"

namespace game
{

TLVReader::TLVReader(std::span<const std::byte> buffer)
    : buffer_(buffer)
{
}

TLVReader::Iterator::Iterator(std::span<const std::byte> buffer)
    : buffer_(buffer)
{
}

auto TLVReader::Iterator::operator*() const -> TLVReader::Iterator::value_type
{
    ensure(buffer_.size() >= sizeof(TLVType) + sizeof(std::uint32_t), "invalid entry size");

    auto type = TLVType{};
    std::memcpy(&type, buffer_.data(), sizeof(type));

    auto length = std::uint32_t{};
    std::memcpy(&length, buffer_.data() + sizeof(type), sizeof(length));

    ensure(buffer_.size() >= sizeof(TLVType) + sizeof(std::uint32_t) + length, "invalid length");

    return {type, buffer_.subspan(sizeof(type) + sizeof(length), length)};
}

auto TLVReader::Iterator::operator++() -> TLVReader::Iterator &
{
    const auto entry = operator*();
    buffer_ = buffer_.last(buffer_.size() - entry.size());
    return *this;
}

auto TLVReader::Iterator::operator++(int) -> TLVReader::Iterator
{
    auto tmp = *this;
    ++*this;
    return tmp;
}

auto TLVReader::Iterator::operator==(const Iterator &other) const -> bool
{
    return (other.buffer_.data() == buffer_.data()) && (other.buffer_.size() == buffer_.size());
}
}
