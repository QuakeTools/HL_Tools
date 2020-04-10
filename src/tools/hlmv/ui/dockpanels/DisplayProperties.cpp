#include "ui/dockpanels/DisplayProperties.hpp"

namespace ui::dockpanels
{
DisplayProperties::DisplayProperties(QWidget* parent)
	: QWidget(parent)
{
	_ui.setupUi(this);
}

DisplayProperties::~DisplayProperties() = default;
}
