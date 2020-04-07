#pragma once

#include <QHash>
#include <QSharedPointer>
#include <QString>
#include <QVector>
#include <QWidget>

#include "ui_ControlPanels.h"

#include "ui/controlpanels/ControlPanelState.hpp"

namespace models::studio
{
class EditableStudioModel;
}

namespace ui
{
class BaseControlPanel;
class ViewerWindow;

class ControlPanels final : public QWidget
{
public:
	ControlPanels(QWidget* parent = nullptr);
	~ControlPanels();

public slots:
	void OnModelChanged(models::studio::EditableStudioModel* model);
	void OnModelRemoved(models::studio::EditableStudioModel* model);

private:
	QHash<QString, QSharedPointer<ControlPanelState>>& GetOrCreateStateForModel(const models::studio::EditableStudioModel* model);

	void ClearStateForModel(const models::studio::EditableStudioModel* model);

	void SaveCurrentState(const models::studio::EditableStudioModel* model);

	void RestoreCurrentState(const models::studio::EditableStudioModel* model);

private:
	Ui_ControlPanels _ui;

	QVector<BaseControlPanel*> _controlPanels;

	models::studio::EditableStudioModel* _currentModel = nullptr;

	QHash<const models::studio::EditableStudioModel*, QHash<QString, QSharedPointer<ControlPanelState>>> _states;
};
}
