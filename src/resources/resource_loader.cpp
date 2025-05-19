#include "resources/resource_loader.h"

#include <filesystem>
#include <string_view>

#include "file.h"

namespace game
{

ResourceLoader::ResourceLoader(const std::filesystem::path &root)
    : root_(root)
{
}

auto ResourceLoader::load(std::string_view name) const -> File
{
    return {root_ / name};
}

}
