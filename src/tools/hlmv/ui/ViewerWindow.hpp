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
	Q_OBJECT

public:
	ViewerWindow(QWidget* parent = nullptr);
	~ViewerWindow();

	QWidget* GetContainer() { return _widget; }

	void SetWorld(game::World* world)
	{
		_world = world;
	}

signals:
	void MouseEvent(QMouseEvent& event);

protected:
	void mousePressEvent(QMouseEvent* event) override final
	{
		emit MouseEvent(*event);
	}

	void mouseReleaseEvent(QMouseEvent* event) override final
	{
		emit MouseEvent(*event);
	}

	void mouseMoveEvent(QMouseEvent* event) override final
	{
		emit MouseEvent(*event);
	}

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	QWidget* _widget;

	std::unique_ptr<graphics::Renderer> _renderer;

	game::World* _world = nullptr;
};
}
