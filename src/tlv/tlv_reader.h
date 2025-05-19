#pragma once

#include <cstddef>
#include <iterator>
#include <span>

#include "tlv/tlv_entry.h"

namespace game
{

/**
 * A Type-Length-Value reader. This is a non-owning view over a buffer of data that contains TLV entries. It provides a
 * forward iterator for iterating over the entries.
 */
class TLVReader
{
  public:
    /**
     * Iterator for iterating over TLV entries. This is a forward iterator that provides access to the entries in the
     * buffer.
     */
    class Iterator
    {
      public:
        using difference_type = std::ptrdiff_t;
        using value_type = TLVEntry;

        Iterator() = default;
        Iterator(std::span<const std::byte> buffer);

        auto operator*() const -> value_type;

        auto operator++() -> Iterator &;

        auto operator++(int) -> Iterator;

        auto operator==(const Iterator &) const -> bool;

      private:
        std::span<const std::byte> buffer_;
    };
    static_assert(std::forward_iterator<Iterator>);

    /**
     * Construct a new TLV reader.
     *
     * @param buffer
     *   The buffer to read from. This is a non-owning view into the buffer.
     */
    TLVReader(std::span<const std::byte> buffer);

    /**
     * Get the begin iterator for the TLV reader.
     *
     * @returns
     *   The begin iterator for the TLV reader.
     */
    auto begin(this auto &&self) -> Iterator
    {
        return {self.buffer_};
    }

    /**
     * Get the end iterator for the TLV reader.
     *
     * @returns
     *   The end iterator for the TLV reader.
     */
    auto end(this auto &&self) -> Iterator
    {
        return {{self.buffer_.data() + self.buffer_.size(), self.buffer_.data() + self.buffer_.size()}};
    }

  private:
    /** The buffer to read from. This is a non-owning view into the buffer. */
    std::span<const std::byte> buffer_;
};

}
