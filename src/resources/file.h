#pragma once

#include <cstddef>
#include <filesystem>
#include <memory>
#include <ranges>
#include <span>
#include <string_view>

#include <Windows.h>

#include "utils/auto_release.h"
#include "utils/error.h"

namespace game
{

/**
 * Enumeration of file creation modes.
 */
enum class CreationMode
{
    /** Open an existing file. */
    OPEN,

    /** Create a new file (or open existing). */
    CREATE
};

/**
 * Abstraction over a file. Internally uses memory mapped files to access the file contents, so data can re returned as
 * a pointer rather than a copy. You need to keep the file open for the duration of the data usage.
 */
class File
{
  public:
    /**
     * Construct a new file.
     *
     * @param path
     *   The path to the file.
     * @param mode
     *   The creation mode of the file.
     */
    File(const std::filesystem::path &path, CreationMode mode = CreationMode::OPEN);

    /**
     * Get the size of the file.
     *
     * @returns
     *   The size of the file in bytes.
     */
    auto size() const -> std::size_t;

    /**
     * Get the contents of the file as a string.
     *
     * @returns
     *   The contents of the file as a string.
     */
    auto as_string() const -> std::string_view;

    /**
     * Get the contents of the file as a span of bytes.
     *
     * @returns
     *   The contents of the file as a span of bytes.
     */
    auto as_data() const -> std::span<const std::byte>;

    /**
     * Write data to the file.
     *
     * @param data
     *   The data to write to the file. Must be a view or range of contiguous bytes (have a size and data pointer).
     */
    template <class T>
    auto write(const T &data) -> void
        requires(sizeof(std::ranges::range_value_t<T>) == 1)
    {
        std::memcpy(map_view_.get(), std::ranges::data(data), std::ranges::size(data));
        ensure(::FlushViewOfFile(map_view_.get(), data.size()) != 0, "failed to flush file");
    }

  private:
    /** Handle to the file. */
    AutoRelease<HANDLE, INVALID_HANDLE_VALUE> handle_;

    /** Handle to the file mapping. */
    AutoRelease<HANDLE, reinterpret_cast<HANDLE>(NULL)> mapping_;

    /** Pointer to the mapped view of the file. */
    std::unique_ptr<void, decltype(&::UnmapViewOfFile)> map_view_;

    /** Size of the file. */
    std::size_t size_;
};
}
