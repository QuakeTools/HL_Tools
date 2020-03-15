#pragma once

#include <QMainWindow>

#include "ui_HLMVMainWindow.h"

namespace ui
{
class MainPanel;

class HLMVMainWindow final : public QMainWindow
{
	Q_OBJECT

public:
	HLMVMainWindow();
	~HLMVMainWindow();

private slots:
	void OnLoadModel();

	void OnOpenOptionsDialog();

private:
	Ui_HLMVMainWindow _ui;

	MainPanel* _mainPanel;
};
}
