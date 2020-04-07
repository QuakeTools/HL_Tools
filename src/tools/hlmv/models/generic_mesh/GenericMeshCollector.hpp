#pragma once

#include "graphics/GraphicsUtilities.hpp"
#include "graphics/IRenderable.hpp"
#include "graphics/RenderContext.hpp"
#include "graphics/SceneContext.hpp"

#include "graphics/api/DeviceBuffer.hpp"
#include "graphics/api/GraphicsDevice.hpp"
#include "graphics/api/Pipeline.hpp"
#include "graphics/api/ResourceFactory.hpp"
#include "graphics/api/ResourceLayout.hpp"
#include "graphics/api/ResourceSet.hpp"

#include "graphics/objects/Mesh.hpp"

#include "utility/MemoryUtilities.hpp"

//TODO: temporary until a proper generic mesh rendererable has been written

namespace models::generic_mesh
{
struct GenericMeshRenderable final : public graphics::IRenderable
{
	GenericMeshRenderable() = default;
	~GenericMeshRenderable() = default;

	graphics::RenderOrderKey GetRenderOrderKey(const glm::vec3& cameraPosition) override
	{
		return {};
	}

	void Render(graphics::api::GraphicsDevice& gd, graphics::SceneContext& sc, const graphics::RenderPass::RenderPass renderPass) override
	{
		const float Size = 100.f;

		std::array<glm::vec3, 4> vertices
		{
			{
				{-Size, -Size, 0.f},
				{-Size, Size, 0.f},
				{Size, Size, 0.f},
				{Size, -Size, 0.f}
			}
		};

		std::array<std::uint16_t, 6> indices
		{
			0, 1, 2,
			2, 3, 0
		};

		auto vertexBuffer = gd.GetResourceFactory().CreateBuffer({sizeof(vertices), graphics::api::BufferUsage::Vertex});

		auto indexBuffer = gd.GetResourceFactory().CreateBuffer({sizeof(indices), graphics::api::BufferUsage::Index});

		gd.UpdateBuffer(vertexBuffer, 0, vertices);
		gd.UpdateBuffer(indexBuffer, 0, indices);

		if (!_pipeline)
		{
			_layout = gd.GetResourceFactory().CreateResourceLayout(
				{
					utility::make_vector(
						graphics::api::ResourceLayoutElementDescription
						{
							"Projection",
							graphics::api::ResourceKind::UniformBuffer,
							graphics::api::ShaderStages::Vertex
						},
						graphics::api::ResourceLayoutElementDescription
						{
							"View",
							graphics::api::ResourceKind::UniformBuffer,
							graphics::api::ShaderStages::Vertex
						}
					)
				});

			auto [vertexShader, fragmentShader] = graphics::LoadShaders(gd.GetResourceFactory(), "Mesh");

			const auto normalDepthStencilState = gd.IsDepthRangeZeroToOne() ?
				graphics::api::DepthStencilStateDescription::DepthOnlyGreaterEqual :
				graphics::api::DepthStencilStateDescription::DepthOnlyLessEqual;

			graphics::api::PipelineDescription description
			{
				graphics::api::BlendStateDescription::SingleDisabled,
				normalDepthStencilState,
				graphics::api::RasterizerStateDescription::CullNone,
				graphics::api::PrimitiveTopology::TriangleList,
				graphics::api::ShaderSetDescription
				{
					{
						graphics::api::VertexLayoutDescription
						{
							{
								graphics::api::VertexElementDescription
								{
									"vsin_Position",
									graphics::api::VertexElementSemantic::Position,
									graphics::api::VertexElementFormat::Float3
								}
							}
						}
					},
					{vertexShader, fragmentShader}
				},
				{_layout}
			};

			_pipeline = gd.GetResourceFactory().CreatePipeline(std::move(description));
		}

		auto resourceSet = gd.GetResourceFactory().CreateResourceSet(
			{
				_layout,
				{
					sc.ProjectionBuffer,
					sc.ViewBuffer
				}
			});

		gd.SetPipeline(_pipeline);

		gd.SetResourceSet(0, resourceSet);

		gd.SetVertexBuffer(0, vertexBuffer);
		gd.SetIndexBuffer(indexBuffer, graphics::api::IndexFormat::UInt16);

		gd.DrawIndexed(indices.size());
	}

private:
	static inline std::shared_ptr<graphics::api::ResourceLayout> _layout{};
	static inline std::shared_ptr<graphics::api::Pipeline> _pipeline{};
};

inline void CollectGenericMesh(graphics::RenderContext& context)
{
	if (context.RenderPass == graphics::RenderPass::Standard)
	{
		context.Renderables.emplace_back(context.Allocator.Allocate<GenericMeshRenderable>());
	}
}
}
