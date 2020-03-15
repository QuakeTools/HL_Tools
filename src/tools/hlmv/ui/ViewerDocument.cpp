#include "ui/ViewerDocument.h"
#include "ui/ViewerWindow.h"

#include "ui/controlpanels/viewer/ModelDisplayPanel.h"

namespace ui
{
ViewerDocument::ViewerDocument(QWidget* parent)
	: QWidget(parent)
{
	_ui.setupUi(this);

	_window = new ViewerWindow(this);

	const auto viewerWindowWidget = _window->GetContainer();

	_ui.Layout->insertWidget(0, viewerWindowWidget, 2);

	viewerWindowWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	_ui.ViewerTabs->addTab(new controlpanels::viewer::ModelDisplayPanel(this), "Model Display");
}

ViewerDocument::~ViewerDocument() = default;
}
