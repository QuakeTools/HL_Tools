#pragma once

#include <memory>
#include <vector>

class QOpenGLWindow;

namespace game
{
class World;
}

namespace graphics
{
class SceneContext;
class SceneRenderer;

namespace api
{
class GraphicsDevice;
}

class Renderer final
{
public:
	Renderer(QOpenGLWindow* window);
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();

	void OnWindowResized(unsigned int width, unsigned int height);

	void Render(game::World* world);

private:
	QOpenGLWindow* _window;
	std::unique_ptr<api::GraphicsDevice> _gd;

	bool _windowResized = false;

	std::unique_ptr<SceneRenderer> _sceneRenderer;

	std::unique_ptr<SceneContext> _sc;
};
}
