#pragma once

#include <memory>

#include <QOpenGLWindow>
#include <QWidget>

namespace game
{
class World;
}

namespace graphics
{
class Renderer;
}

namespace ui
{
class ViewerWindow final : public QOpenGLWindow
{
public:
	ViewerWindow(QWidget* parent = nullptr);
	~ViewerWindow();

	QWidget* GetContainer() { return _widget; }

	void SetWorld(game::World* world)
	{
		_world = world;
	}

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	QWidget* _widget;

	std::unique_ptr<graphics::Renderer> _renderer;

	game::World* _world = nullptr;
};
}
