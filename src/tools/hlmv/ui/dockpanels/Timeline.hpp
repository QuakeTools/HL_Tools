#pragma once

#include <QWidget>

#include "ui_Timeline.h"

namespace ui::dockpanels
{
struct Timeline final : public QWidget
{
	Timeline(QWidget* parent = nullptr)
		: QWidget(parent)
	{
		_ui.setupUi(this);

		auto style = this->style();

		_ui.Restart->setIcon(style->standardIcon(QStyle::StandardPixmap::SP_BrowserReload));
		_ui.PreviousFrame->setIcon(style->standardIcon(QStyle::StandardPixmap::SP_MediaSeekBackward));
		_ui.SkipFramesEarlier->setIcon(style->standardIcon(QStyle::StandardPixmap::SP_MediaSkipBackward));
		_ui.TogglePlayback->setIcon(style->standardIcon(QStyle::StandardPixmap::SP_MediaPlay));
		_ui.SkipFramesLater->setIcon(style->standardIcon(QStyle::StandardPixmap::SP_MediaSkipForward));
		_ui.NextFrame->setIcon(style->standardIcon(QStyle::StandardPixmap::SP_MediaSeekForward));
	}

	~Timeline() = default;

private:
	Ui_Timeline _ui;
};
}
