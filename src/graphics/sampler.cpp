#include "graphics/sampler.h"

#include "graphics/opengl.h"
#include "utils/auto_release.h"

namespace game
{

Sampler::Sampler()
    : handle_{0u, [](auto sampler) { ::glDeleteSamplers(1, &sampler); }}
{
    ::glCreateSamplers(1, &handle_);
}

auto Sampler::native_handle() const -> ::GLuint
{
    return handle_;
}

}
