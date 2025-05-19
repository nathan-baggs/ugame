#pragma once

#include "graphics/opengl.h"
#include "utils/auto_release.h"

namespace game
{

/**
 * This class encapsulates how a texture should be samples. Currently no implementation other than the OpenGL defaults.
 */
class Sampler
{
  public:
    /**
     * Construct a Sampler.
     */
    Sampler();

    /**
     * Get the native OpenGL sampler handle.
     *
     * @returns
     *   The native OpenGL sampler handle.
     */
    auto native_handle() const -> ::GLuint;

  private:
    /**  OpenGL sampler handle. */
    AutoRelease<::GLuint> handle_;
};
}
