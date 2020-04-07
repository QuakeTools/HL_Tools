#pragma once

#include <tuple>

#include <GL/glew.h>

#include "graphics/api/Enums.hpp"

namespace graphics::api::opengl::detail
{
GLenum BufferUsageToTarget(std::uint8_t usage);

GLenum BufferUsageToUsage(std::uint8_t usage);

GLenum ShaderStagesToType(ShaderStages::ShaderStages stage);

GLenum TextureTypeToType(const TextureType type);

GLenum PixelFormatToFormat(const PixelFormat format);

GLenum PixelFormatToInternalFormat(const PixelFormat format);

GLenum PixelFormatToPixelType(const PixelFormat format);

GLenum IndexFormatToType(const IndexFormat format);

GLenum VertexElementFormatToAttrib(const VertexElementFormat format, bool& isInteger);

std::tuple<GLint, GLint> SamplerFilterToTextureMinMagFilter(const SamplerFilter filter, bool mip);

GLenum VdToGLBlendFactorSrc(const BlendFactor factor);

GLenum VdToGLBlendFactorDest(const BlendFactor factor);

GLenum VdToGLBlendEquationMode(const BlendFunction function);

GLenum VdToGLDepthFunction(const ComparisonKind value);

GLenum VdToGLStencilFunction(const ComparisonKind comparison);

GLenum VdToGLCullFaceMode(const FaceCullMode cullMode);

GLenum VdToGLPolygonMode(const PolygonFillMode fillMode);

GLenum VdToGLFrontFaceDirection(const FrontFace frontFace);

GLenum VdToGLPrimitiveType(const PrimitiveTopology primitiveTopology);
}
