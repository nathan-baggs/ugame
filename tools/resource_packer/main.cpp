#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <print>
#include <ranges>
#include <set>

#include <assimp/Importer.hpp>
#include <assimp/Logger.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>

#include <stb_image.h>

#include "graphics/mesh_data.h"
#include "graphics/texture.h"
#include "graphics/vertex_data.h"
#include "maths/vector3.h"
#include "tlv/tlv_writer.h"
#include "utils/auto_release.h"
#include "utils/error.h"
#include "utils/exception.h"
#include "utils/log.h"

namespace
{
game::TextureFormat to_texture_format(int num_channels)
{
    switch (num_channels)
    {
        case 3: return game::TextureFormat::RGB;
        case 4: return game::TextureFormat::RGBA;
        default: throw game::Exception("unsupported number of channels: {}", num_channels);
    }
}

game::TextureUsage to_texture_usage(std::string_view path)
{
    if (path.contains(".data"))
    {
        return game::TextureUsage::DATA;
    }
    else if (path.contains(".srgb"))
    {
        return game::TextureUsage::SRGB;
    }

    throw game::Exception("unsupported usage type: {}", path);
}
}

auto main(int argc, char **argv) -> int
{
    try
    {
        std::println("resource packer");

        game::ensure(argc == 3, "usage: ./resource_packer.exe <asset_dir> <out_path>");

        const auto image_extensions = std::set<std::string>{".png", ".jpg"};

        auto writer = game::TLVWriter{};

        for (const auto &entry : std::filesystem::directory_iterator{argv[1]})
        {
            const auto path = entry.path().string();
            const auto ext = entry.path().extension().string();
            const auto filename = entry.path().filename().string();
            const auto asset_name = filename.substr(0, filename.find("."));

            if (image_extensions.contains(ext))
            {
                auto w = int{};
                auto h = int{};
                auto num_channels = int{};

                game::ensure(::stbi_info(path.c_str(), &w, &h, &num_channels) == 1, "failed to get image info");
                auto raw_data = std::unique_ptr<::stbi_uc, void (*)(void *)>(
                    ::stbi_load(path.c_str(), &w, &h, &num_channels, 0), ::stbi_image_free);

                std::println("packing path: {} {} {} {} {}", asset_name, ext, w, h, num_channels);

                writer.write(
                    asset_name,
                    w,
                    h,
                    to_texture_format(num_channels),
                    to_texture_usage(path),
                    {reinterpret_cast<const std::byte *>(raw_data.get()),
                     static_cast<std::size_t>(w * h * num_channels)});
            }
            else if (ext == ".obj")
            {
                auto stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
                aiAttachLogStream(&stream);

                auto importer = ::Assimp::Importer{};
                const auto *scene = importer.ReadFile(
                    path.c_str(), ::aiProcess_Triangulate | ::aiProcess_FlipUVs | ::aiProcess_CalcTangentSpace);

                game::ensure((scene != nullptr), "failed to load model {}", path);

                const auto loaded_meshes = std::span<::aiMesh *>(scene->mMeshes, scene->mMeshes + scene->mNumMeshes);

                for (const auto *mesh : loaded_meshes)
                {
                    game::log::info("packing {}", mesh->mName.C_Str());

                    const auto to_vector3 = [](const ::aiVector3D &v) { return game::Vector3{v.x, v.y, v.z}; };
                    const auto positions =
                        std::span<::aiVector3D>{mesh->mVertices, mesh->mVertices + mesh->mNumVertices} |
                        std::views::transform(to_vector3);
                    const auto normals = std::span<::aiVector3D>{mesh->mNormals, mesh->mNormals + mesh->mNumVertices} |
                                         std::views::transform(to_vector3);

                    auto uvs = std::vector<game::UV>{};
                    auto tangents = std::vector<game::Vector3>{};
                    for (auto i = 0u; i < mesh->mNumVertices; ++i)
                    {
                        uvs.push_back({mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y});
                        tangents.push_back({mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z});
                    }

                    auto indices = std::vector<std::uint32_t>{};
                    for (auto i = 0u; i < mesh->mNumFaces; ++i)
                    {
                        const auto &face = mesh->mFaces[i];
                        for (auto j = 0u; j < face.mNumIndices; ++j)
                        {
                            indices.push_back(face.mIndices[j]);
                        }
                    }

                    const auto vertices =
                        std::views::zip_transform(
                            []<class... A>(A &&...a) { return game::VertexData{std::forward<A>(a)...}; },
                            positions,
                            normals,
                            tangents,
                            uvs) |
                        std::ranges::to<std::vector>();

                    writer.write(mesh->mName.C_Str(), vertices, indices);
                }
            }
        }

        const auto resource_data = writer.yield();

        game::log::info("writing resource {} bytes", resource_data.size());

        std::ofstream out{argv[2], std::ios::binary};
        out.write(reinterpret_cast<const char *>(resource_data.data()), resource_data.size());
    }
    catch (game::Exception &e)
    {
        std::println(std::cerr, "{}", e);
    }

    return 0;
}
