#pragma once

#include <QWidget>

#include "ui_ModelDisplayPanel.h"

#include "ui/controlpanels/BaseControlPanel.hpp"

namespace ui::controlpanels::viewer
{
class ModelDisplayPanel final : public BaseControlPanel
{
	Q_OBJECT

public:
	ModelDisplayPanel(QWidget* parent = nullptr);
	~ModelDisplayPanel();

	void SaveState(ControlPanelState& state) const override;

	void RestoreState(const ControlPanelState& state) override;

private:
	Ui_ModelDisplayPanel _ui;
};
}
