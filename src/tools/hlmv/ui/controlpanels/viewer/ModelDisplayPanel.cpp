#include "ui/controlpanels/viewer/ModelDisplayPanel.hpp"

namespace ui::controlpanels::viewer
{
ModelDisplayPanel::ModelDisplayPanel(QWidget* parent)
	: BaseControlPanel(parent)
{
	_ui.setupUi(this);
}

ModelDisplayPanel::~ModelDisplayPanel() = default;

void ModelDisplayPanel::SaveState(ControlPanelState& state) const
{
}

void ModelDisplayPanel::RestoreState(const ControlPanelState& state)
{
}
}
