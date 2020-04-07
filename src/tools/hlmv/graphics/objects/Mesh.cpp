#include <stdexcept>

#include "graphics/api/GraphicsAPIHelpers.hpp"
#include "graphics/api/GraphicsDevice.hpp"
#include "graphics/api/ResourceFactory.hpp"
#include "graphics/objects/Mesh.hpp"

using namespace graphics::api;

namespace graphics::objects
{
std::size_t Mesh::GetElementCountForSemantic(VertexElementSemantic semantic) const
{
	switch (semantic)
	{
	case VertexElementSemantic::Position:
	case VertexElementSemantic::Normal: return 3;

	case VertexElementSemantic::TextureCoordinate: return 2;
	case VertexElementSemantic::Color: return 4;

	case VertexElementSemantic::BoneIndex: return 1;

	default: throw std::invalid_argument("Unknown semantic");
	}
}

void Mesh::Clear()
{
	for (auto& element : _elements)
	{
		element.Data.clear();
		element.InUse = false;
	}

	_vertexBuffer.reset();
	_indexBuffer.reset();
}

void Mesh::UpdateDeviceResources(graphics::api::GraphicsDevice& gd, SceneContext& sc)
{
	//For now just recreate all data
	std::size_t singleVertexSize = 0;

	std::array<std::size_t, 5> offsets;

	for (const auto& element : _elements)
	{
		if (element.InUse)
		{
			offsets[static_cast<std::size_t>(element.Semantic)] = singleVertexSize;

			singleVertexSize += VertexElementFormatToSizeInBytes(element.Format);
		}
	}

	const std::size_t elementCount = _elements[static_cast<std::size_t>(VertexElementSemantic::Position)].Data.size() / sizeof(glm::vec3);

	const std::size_t totalBufferSize = singleVertexSize * elementCount;

	auto data = std::make_unique<std::byte[]>(totalBufferSize);

	for (const auto& element : _elements)
	{
		if (element.InUse)
		{
			std::size_t offset = offsets[static_cast<std::size_t>(element.Semantic)];

			const std::size_t elementSizeInBytes = VertexElementFormatToSizeInBytes(element.Format);

			for (std::size_t index = 0U; index < elementCount; ++index)
			{
				std::memcpy(data.get() + offset, element.Data.data() + (index * elementSizeInBytes), elementSizeInBytes);

				offset += singleVertexSize;
			}
		}
	}

	auto& factory = gd.GetResourceFactory();

	_vertexBuffer = factory.CreateBuffer({totalBufferSize, BufferUsage::Vertex});

	gd.UpdateBuffer(_vertexBuffer, 0U, data.get(), totalBufferSize);

	//TODO: make it possible to modify the index format
	_indexBuffer = factory.CreateBuffer({_indices.size() * sizeof(std::uint32_t), BufferUsage::Index});

	gd.UpdateBuffer(_indexBuffer, 0U, _indices);
}
}
