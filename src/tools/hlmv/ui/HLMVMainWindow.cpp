#include "ui/HLMVMainWindow.hpp"
#include "ui/MainPanel.hpp"

#include "ui/options/OptionsDialog.hpp"

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

	connect(_ui.ActionLoadModel, &QAction::triggered, _mainPanel, &MainPanel::PromptLoadModel);
	connect(_ui.ActionOptions, &QAction::triggered, this, &HLMVMainWindow::OnOpenOptionsDialog);
}

HLMVMainWindow::~HLMVMainWindow() = default;

void HLMVMainWindow::OnOpenOptionsDialog()
{
	options::OptionsDialog dialog(this);

	dialog.exec();
}
}
