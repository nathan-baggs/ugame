#pragma once

#include <Windows.h>
#include <gl/gl.h>

#include "third_party/opengl/glext.h"

// x-macro for iterating over opengl function signatures and names
#define FOR_OPENGL_FUNCTIONS(DO)                                                                                       \
    DO(::PFNGLCREATESHADERPROC, glCreateShader)                                                                        \
    DO(::PFNGLDELETESHADERPROC, glDeleteShader)                                                                        \
    DO(::PFNGLSHADERSOURCEPROC, glShaderSource)                                                                        \
    DO(::PFNGLCOMPILESHADERPROC, glCompileShader)                                                                      \
    DO(::PFNGLGETSHADERIVPROC, glGetShaderiv)                                                                          \
    DO(::PFNGLGETPROGRAMIVPROC, glGetProgramiv)                                                                        \
    DO(::PFNGLCREATEPROGRAMPROC, glCreateProgram)                                                                      \
    DO(::PFNGLDELETEPROGRAMPROC, glDeleteProgram)                                                                      \
    DO(::PFNGLATTACHSHADERPROC, glAttachShader)                                                                        \
    DO(::PFNGLLINKPROGRAMPROC, glLinkProgram)                                                                          \
    DO(::PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays)                                                                  \
    DO(::PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays)                                                            \
    DO(::PFNGLGENBUFFERSPROC, glGenBuffers)                                                                            \
    DO(::PFNGLDELETEBUFFERSPROC, glDeleteBuffers)                                                                      \
    DO(::PFNGLBINDVERTEXARRAYPROC, glBindVertexArray)                                                                  \
    DO(::PFNGLBINDBUFFERPROC, glBindBuffer)                                                                            \
    DO(::PFNGLBUFFERDATAPROC, glBufferData)                                                                            \
    DO(::PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer)                                                          \
    DO(::PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray)                                                  \
    DO(::PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback)                                                        \
    DO(::PFNGLUSEPROGRAMPROC, glUseProgram)                                                                            \
    DO(::PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation)                                                            \
    DO(::PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv)                                                                \
    DO(::PFNGLUNIFORM3FVPROC, glUniform3fv)                                                                            \
    DO(::PFNGLUNIFORM1IPROC, glUniform1i)                                                                              \
    DO(::PFNGLUNIFORM1FPROC, glUniform1f)                                                                              \
    DO(::PFNGLCREATEBUFFERSPROC, glCreateBuffers)                                                                      \
    DO(::PFNGLNAMEDBUFFERSTORAGEPROC, glNamedBufferStorage)                                                            \
    DO(::PFNGLCREATEVERTEXARRAYSPROC, glCreateVertexArrays)                                                            \
    DO(::PFNGLVERTEXARRAYBINDVERTEXBUFFEREXTPROC, glVertexArrayVertexBuffer)                                           \
    DO(::PFNGLENABLEVERTEXARRAYATTRIBPROC, glEnableVertexArrayAttrib)                                                  \
    DO(::PFNGLVERTEXARRAYATTRIBFORMATPROC, glVertexArrayAttribFormat)                                                  \
    DO(::PFNGLVERTEXARRAYATTRIBBINDINGPROC, glVertexArrayAttribBinding)                                                \
    DO(::PFNGLNAMEDBUFFERSUBDATAPROC, glNamedBufferSubData)                                                            \
    DO(::PFNGLVERTEXARRAYELEMENTBUFFERPROC, glVertexArrayElementBuffer)                                                \
    DO(::PFNGLBINDBUFFERBASEPROC, glBindBufferBase)                                                                    \
    DO(::PFNGLCREATETEXTURESPROC, glCreateTextures)                                                                    \
    DO(::PFNGLTEXTURESTORAGE2DPROC, glTextureStorage2D)                                                                \
    DO(::PFNGLTEXTURESUBIMAGE2DPROC, glTextureSubImage2D)                                                              \
    DO(::PFNGLTEXTURESUBIMAGE3DPROC, glTextureSubImage3D)                                                              \
    DO(::PFNGLCREATESAMPLERSPROC, glCreateSamplers)                                                                    \
    DO(::PFNGLDELETESAMPLERSPROC, glDeleteSamplers)                                                                    \
    DO(::PFNGLBINDTEXTUREUNITPROC, glBindTextureUnit)                                                                  \
    DO(::PFNGLBINDSAMPLERPROC, glBindSampler)                                                                          \
    DO(::PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)                                                                \
    DO(::PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)                                                              \
    DO(::PFNGLGETACTIVEUNIFORMPROC, glGetActiveUniform)                                                                \
    DO(::PFNGLCREATEFRAMEBUFFERSPROC, glCreateFramebuffers)                                                            \
    DO(::PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers)                                                            \
    DO(::PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer)                                                                  \
    DO(::PFNGLNAMEDFRAMEBUFFERTEXTUREPROC, glNamedFramebufferTexture)                                                  \
    DO(::PFNGLBLITNAMEDFRAMEBUFFERPROC, glBlitNamedFramebuffer)                                                        \
    DO(::PFNGLDRAWARRAYSEXTPROC, glDrawArraysEXT)

// expand x-macro to define function pointers
#define DO_DEFINE(TYPE, NAME) inline TYPE NAME;
FOR_OPENGL_FUNCTIONS(DO_DEFINE)
