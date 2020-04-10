#pragma once

#include <QWidget>

#include "ui_SceneInfo.h"

namespace ui::dockpanels
{
struct SceneInfo final : public QWidget
{
	SceneInfo(QWidget* parent = nullptr)
		: QWidget(parent)
	{
		_ui.setupUi(this);
	}

	~SceneInfo() = default;

private:
	Ui_SceneInfo _ui;
};
}
