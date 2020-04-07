#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "graphics/api/DeviceBuffer.hpp"
#include "graphics/api/Enums.hpp"

namespace graphics
{
class SceneContext;

namespace api
{
class GraphicsDevice;
}

namespace objects
{
/**
*	@brief A mutable representation of a 3D mesh
*/
class Mesh final
{
public:
	Mesh() = default;
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	~Mesh() = default;

	std::uint32_t GetVersion() const { return _version; }

	std::size_t GetElementCountForSemantic(graphics::api::VertexElementSemantic semantic) const;

	void Clear();

	std::vector<glm::vec3> GetVertices() const
	{
		return GetElementArrayForSemantic<glm::vec3>(graphics::api::VertexElementSemantic::Position);
	}

	void SetVertices(const std::vector<glm::vec3>& data)
	{
		SetElementArrayForSemantic(graphics::api::VertexElementSemantic::Position, data);
	}

	std::vector<glm::vec3> GetNormals() const
	{
		return GetElementArrayForSemantic<glm::vec3>(graphics::api::VertexElementSemantic::Normal);
	}

	void SetNormals(const std::vector<glm::vec3>& data)
	{
		SetElementArrayForSemantic(graphics::api::VertexElementSemantic::Normal, data);
	}

	std::vector<glm::vec2> GetTextureCoordinates() const
	{
		return GetElementArrayForSemantic<glm::vec2>(graphics::api::VertexElementSemantic::TextureCoordinate);
	}

	void SetTextureCoordinates(const std::vector<glm::vec2>& data)
	{
		SetElementArrayForSemantic(graphics::api::VertexElementSemantic::TextureCoordinate, data);
	}

	std::vector<glm::vec4> GetColors() const
	{
		return GetElementArrayForSemantic<glm::vec4>(graphics::api::VertexElementSemantic::Color);
	}

	void SetColors(const std::vector<glm::vec4>& data)
	{
		SetElementArrayForSemantic(graphics::api::VertexElementSemantic::Color, data);
	}

	std::vector<std::int32_t> GetBoneIndices() const
	{
		return GetElementArrayForSemantic<std::int32_t>(graphics::api::VertexElementSemantic::BoneIndex);
	}

	void SetBoneIndices(const std::vector<std::int32_t>& data)
	{
		SetElementArrayForSemantic(graphics::api::VertexElementSemantic::BoneIndex, data);
	}

	graphics::api::IndexFormat GetIndexFormat() const { return graphics::api::IndexFormat::UInt32; }

	std::vector<std::uint32_t> GetIndices() const { return _indices; }

	void SetIndices(const std::vector<std::uint32_t>& indices)
	{
		_indices = indices;
	}

	const std::shared_ptr<graphics::api::DeviceBuffer>& GetVertexBuffer() const { return _vertexBuffer; }

	const std::shared_ptr<graphics::api::DeviceBuffer>& GetIndexBuffer() const { return _indexBuffer; }

	void UpdateDeviceResources(graphics::api::GraphicsDevice& gd, SceneContext& sc);

private:
	template<typename TDataType>
	std::vector<TDataType> GetElementArrayForSemantic(graphics::api::VertexElementSemantic semantic) const
	{
		const auto& element = _elements[static_cast<std::size_t>(semantic)];

		if (element.InUse)
		{
			std::vector<TDataType> data;

			data.resize(element.Data.size() / sizeof(TDataType));

			std::memcpy(data.data(), element.Data.data(), element.Data.size());

			return data;
		}

		return {};
	}

	template<typename TDataType>
	void SetElementArrayForSemantic(graphics::api::VertexElementSemantic semantic, const std::vector<TDataType>& data)
	{
		auto& element = _elements[static_cast<std::size_t>(semantic)];

		element.InUse = true;

		element.Data.resize(data.size() * sizeof(TDataType));

		std::memcpy(element.Data.data(), data.data(), element.Data.size());

		++_version;
	}

private:
	struct VertexElement
	{
		graphics::api::VertexElementSemantic Semantic;
		graphics::api::VertexElementFormat Format;
		std::vector<std::byte> Data{};
		bool InUse = false;
	};

	std::array<VertexElement, 5> _elements
	{
		{
			{graphics::api::VertexElementSemantic::Position, graphics::api::VertexElementFormat::Float3},
			{graphics::api::VertexElementSemantic::Normal, graphics::api::VertexElementFormat::Float3},
			{graphics::api::VertexElementSemantic::TextureCoordinate, graphics::api::VertexElementFormat::Float2},
			{graphics::api::VertexElementSemantic::Color, graphics::api::VertexElementFormat::Float4},
			{graphics::api::VertexElementSemantic::BoneIndex, graphics::api::VertexElementFormat::Int1}
		}
	};

	std::vector<std::uint32_t> _indices;

	std::shared_ptr<graphics::api::DeviceBuffer> _vertexBuffer;
	std::shared_ptr<graphics::api::DeviceBuffer> _indexBuffer;

	//Indicates that this mesh's data has been changed since the last time device buffers were created
	std::int32_t _version{};
};
}
}
