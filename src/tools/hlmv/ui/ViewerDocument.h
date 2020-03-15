#pragma once

#include <QWidget>

#include "ui_ViewerDocument.h"

namespace ui
{
class ViewerWindow;

class ViewerDocument final : public QWidget
{
public:
	ViewerDocument(QWidget* parent = nullptr);
	~ViewerDocument();

private:
	Ui_ViewerDocument _ui;

	ViewerWindow* _window;
};
}
