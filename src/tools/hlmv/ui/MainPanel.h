#pragma once

#include <QString>
#include <QWidget>

#include "ui_MainPanel.h"

namespace ui
{
class MainPanel final : public QWidget
{
public:
	MainPanel(QWidget* parent = nullptr);
	~MainPanel();

	//TODO: needs to pass the model/entity
	void AddDocumentForModel(const QString& fileName);

private:
	Ui_MainPanel _ui;
};
}
