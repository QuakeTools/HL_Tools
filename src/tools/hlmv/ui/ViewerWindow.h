#pragma once

#include <QOpenGLWindow>
#include <QWidget>

namespace ui
{
class ViewerWindow final : public QOpenGLWindow
{
public:
	ViewerWindow(QWidget* parent = nullptr);

	QWidget* GetContainer() { return _widget; }

private:
	QWidget* _widget;
};
}
