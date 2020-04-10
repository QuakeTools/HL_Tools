#include <GL/glew.h>

#include <QOpenGLWindow>

#include "game/World.hpp"

#include "graphics/Renderer.hpp"
#include "graphics/Scene.hpp"
#include "graphics/SceneContext.hpp"
#include "graphics/SceneRenderer.hpp"

#include "graphics/api/GraphicsDevice.hpp"
#include "graphics/api/opengl/OpenGLGraphicsDevice.hpp"

namespace graphics
{
Renderer::Renderer(QOpenGLWindow* window)
	: _window(window)
{
	api::GraphicsDeviceOptions options{};

	_gd = std::make_unique<api::opengl::OpenGLGraphicsDevice>(window->width(), window->height(), options);

	_sceneRenderer = std::make_unique<SceneRenderer>();

	_sc = std::make_unique<SceneContext>();

	_sc->CreateDeviceResources(*_gd);
}

Renderer::~Renderer()
{
	_sc->DestroyDeviceResources();
}

void Renderer::OnWindowResized(unsigned int width, unsigned int height)
{
	_windowResized = true;
}

void Renderer::Render(game::World* world)
{
	if (_windowResized)
	{
		_windowResized = false;

		const auto width = _window->width();
		const auto height = _window->height();

		_gd->ResizeMainWindow(width, height);

		_sc->RecreateWindowSizedResources(*_gd);
	}

	_gd->SetFullViewport();
	_gd->SetFullScissorRect();

	//Clear entire target to empty
	_gd->ClearColorTarget({0, 0, 0, 1});

	if (world)
	{
		auto& registry = *world->GetRegistry();

		auto& scene = registry.ctx<Scene>();

		_sceneRenderer->RenderAllStages(registry, scene, *_gd, *_sc);
	}

	//Don't leave a dangling pointer
	_sc->SetScene(nullptr);
}
}
