#pragma once

#include <QWidget>

#include "ui_RenderProperties.h"

namespace ui::dockpanels
{
struct RenderProperties final : public QWidget
{
	RenderProperties(QWidget* parent = nullptr)
		: QWidget(parent)
	{
		_ui.setupUi(this);
	}

	~RenderProperties() = default;

private:
	Ui_RenderProperties _ui;
};
}
