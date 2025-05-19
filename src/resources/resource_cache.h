#pragma once

#include <ranges>
#include <string_view>
#include <tuple>

#include "utils/error.h"
#include "utils/string_map.h"

namespace game
{

class Mesh;
class Texture;
class Material;
class Sampler;

/**
 * A generic resource cache. Allows you to store string keys against the templated types. Keys must be unique per type.
 */
template <class... T>
class ResourceCache
{
  public:
    /**
     * Insert an object into the cache (of the given type). Undefined behaviour if the object already exists.
     *
     * @param name
     *   Name of object to insert.
     *
     * @param args
     *   Arguments to pass to the constructor of the object.
     */
    template <class U, class... Args>
    auto insert(std::string_view name, Args &&...args) -> U *
    {
        auto &map = std::get<StringMap<U>>(maps_);

        expect(!map.contains(name), "{} already exists", name);
        const auto [iter, inserted] = map.emplace(std::make_pair(name, U{std::forward<Args>(args)...}));

        expect(inserted, "insertion failed as key exists");

        return std::addressof(iter->second);
    }

    /**
     * Get an object.
     *
     * @param name
     *   Name of object to get, undefined behaviour if it doesn't exist.
     *
     * @returns
     *   Pointer to requested object.
     */
    template <class U>
    auto get(std::string_view name)
    {
        auto &map = std::get<StringMap<U>>(maps_);

        const auto object = map.find(name);
        expect(object != std::ranges::end(map), "{} does not exist", name);

        return std::addressof(object->second);
    }

  private:
    /** Object store for given types. */
    std::tuple<StringMap<T>...> maps_;
};

// default cache for the game
using DefaultCache = ResourceCache<Mesh, Material, Texture, Sampler>;

}
