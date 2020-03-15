#pragma once

#include <QWidget>

#include "ui_ModelDisplayPanel.h"

namespace ui::controlpanels::viewer
{
class ModelDisplayPanel final : public QWidget
{
public:
	ModelDisplayPanel(QWidget* parent = nullptr);
	~ModelDisplayPanel();

private:
	Ui_ModelDisplayPanel _ui;
};
}
