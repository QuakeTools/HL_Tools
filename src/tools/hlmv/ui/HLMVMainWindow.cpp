#include <QFileDialog>

#include "ui/HLMVMainWindow.h"
#include "ui/MainPanel.h"

#include "ui/options/OptionsDialog.h"

namespace ui
{
HLMVMainWindow::HLMVMainWindow()
	: QMainWindow()
{
	_ui.setupUi(this);

	//The main window should be deleted automatically
	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose, true);

	_mainPanel = new MainPanel(this);

	setCentralWidget(_mainPanel);

	connect(_ui.ActionLoadModel, &QAction::triggered, this, &HLMVMainWindow::OnLoadModel);
	connect(_ui.ActionOptions, &QAction::triggered, this, &HLMVMainWindow::OnOpenOptionsDialog);
}

HLMVMainWindow::~HLMVMainWindow() = default;

void HLMVMainWindow::OnLoadModel()
{
	if (const auto fileName = QFileDialog::getOpenFileName(this, "Select model", {}, "Studio model files (*.mdl *.dol);;All files (*.*)");
		!fileName.isEmpty())
	{
		//TODO: load model
		_mainPanel->AddDocumentForModel(fileName);
	}
}

void HLMVMainWindow::OnOpenOptionsDialog()
{
	options::OptionsDialog dialog(this);

	dialog.exec();
}
}
