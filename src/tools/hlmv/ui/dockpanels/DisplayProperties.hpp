#pragma once

#include <QWidget>

#include "ui_DisplayProperties.h"

namespace ui::dockpanels
{
struct DisplayProperties final : public QWidget
{
	DisplayProperties(QWidget* parent = nullptr);
	~DisplayProperties();

private:
	Ui_DisplayProperties _ui;
};
}
