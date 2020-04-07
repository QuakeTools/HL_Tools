#pragma once

#include <QWidget>

namespace ui
{
class ControlPanelState;

/**
*	@brief Base class for control panels
*	@details
*	Deriving classes must use the Q_OBJECT macro to define a meta object for the class
*/
class BaseControlPanel : public QWidget
{
public:
	BaseControlPanel(QWidget* parent = nullptr)
		: QWidget(parent)
	{
	}

	~BaseControlPanel() = default;

	virtual void SaveState(ControlPanelState& state) const = 0;

	virtual void RestoreState(const ControlPanelState& state) = 0;
};
}
