#include <stdexcept>

#include "graphics/api/ResourceLayout.hpp"
#include "graphics/api/ResourceSet.hpp"

#include "graphics/api/opengl/OpenGLDeviceBuffer.hpp"
#include "graphics/api/opengl/OpenGLPipeline.hpp"
#include "graphics/api/opengl/OpenGLResourceFactory.hpp"
#include "graphics/api/opengl/OpenGLShader.hpp"
#include "graphics/api/opengl/OpenGLTexture.hpp"
#include "graphics/api/opengl/OpenGLUtils.hpp"

using namespace graphics::api::opengl::detail;

namespace graphics::api::opengl
{
std::shared_ptr<DeviceBuffer> OpenGLResourceFactory::CreateBuffer(const BufferDescription& description)
{
	return std::shared_ptr<DeviceBuffer>{ new OpenGLDeviceBuffer(description) };
}

std::shared_ptr<ResourceLayout> OpenGLResourceFactory::CreateResourceLayout(ResourceLayoutDescription&& description)
{
	return std::shared_ptr<ResourceLayout>(new ResourceLayout(std::move(description)));
}

std::shared_ptr<ResourceSet> OpenGLResourceFactory::CreateResourceSet(ResourceSetDescription&& description)
{
	//Validate that the correct resources have been given
	if (description.Layout->GetElements().size() != description.Resources.size())
	{
		throw std::invalid_argument("Resource count does not match layout");
	}

	for (size_t i = 0; i < description.Layout->GetElements().size(); ++i)
	{
		switch (description.Layout->GetElements()[i].Kind)
		{
		case ResourceKind::UniformBuffer:
			AssertSubtype<OpenGLDeviceBuffer>(*description.Resources[i]);
			break;

		case ResourceKind::TextureReadOnly:
			AssertSubtype<OpenGLTexture>(*description.Resources[i]);
			break;
		}
	}

	return std::shared_ptr<ResourceSet>(new ResourceSet(std::move(description)));
}

std::shared_ptr<Shader> OpenGLResourceFactory::CreateShader(const ShaderDescription& description)
{
	return std::shared_ptr<Shader>(new OpenGLShader(description));
}

std::shared_ptr<Pipeline> OpenGLResourceFactory::CreatePipeline(PipelineDescription&& description)
{
	return std::shared_ptr<Pipeline>(new OpenGLPipeline(std::move(description)));
}

std::shared_ptr<Texture> OpenGLResourceFactory::CreateTexture(const TextureDescription& description)
{
	return std::shared_ptr<Texture>(new OpenGLTexture(description));
}
}
