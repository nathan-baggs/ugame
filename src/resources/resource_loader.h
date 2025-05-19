#pragma once

#include <cstddef>
#include <filesystem>
#include <string_view>
#include <vector>

#include "file.h"

namespace game
{

/**
 * Simple resource loader that loads files from a given root directory.
 */
class ResourceLoader
{
  public:
    /**
     * Construct a new resource loader.
     *
     * @param root
     *   The root directory to load resources from.
     */
    ResourceLoader(const std::filesystem::path &root);

    /**
     * Load a file from the resource directory. It is undefined behaviour if the file does not exist.
     *
     * @param name
     *
     * @returns
     *   The file object.
     */
    auto load(std::string_view name) const -> File;

  private:
    /** The root directory to load resources from. */
    std::filesystem::path root_;
};

}
