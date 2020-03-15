#include "ui/controlpanels/viewer/ModelDisplayPanel.h"

namespace ui::controlpanels::viewer
{
ModelDisplayPanel::ModelDisplayPanel(QWidget* parent)
	: QWidget(parent)
{
	_ui.setupUi(this);
}

ModelDisplayPanel::~ModelDisplayPanel() = default;
}
