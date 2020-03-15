#include "ui/MainPanel.h"

#include "ui/ViewerDocument.h"

namespace ui
{
MainPanel::MainPanel(QWidget* parent)
	: QWidget(parent)
{
	_ui.setupUi(this);
}

MainPanel::~MainPanel() = default;

void MainPanel::AddDocumentForModel(const QString& fileName)
{
	_ui.Documents->addTab(new ViewerDocument(this), fileName);
}
}
