#include "ui/ControlPanels.hpp"

#include "ui/controlpanels/viewer/ModelDisplayPanel.hpp"

namespace ui
{
ControlPanels::ControlPanels(QWidget* parent)
	: QWidget(parent)
{
	_ui.setupUi(this);

	auto modelDisplay = new controlpanels::viewer::ModelDisplayPanel(this);

	_controlPanels.append(modelDisplay);

	_ui.ViewerTabs->addTab(modelDisplay, "Model Display");
}

ControlPanels::~ControlPanels() = default;

void ControlPanels::OnModelChanged(models::studio::EditableStudioModel* model)
{
	SaveCurrentState(_currentModel);

	_currentModel = model;

	RestoreCurrentState(_currentModel);
}

void ControlPanels::OnModelRemoved(models::studio::EditableStudioModel* model)
{
	if (_currentModel == model)
	{
		_currentModel = nullptr;

		//Restore to null state; owner will tell us to change to another model
		RestoreCurrentState(nullptr);
	}

	ClearStateForModel(model);
}

QHash<QString, QSharedPointer<ControlPanelState>>& ControlPanels::GetOrCreateStateForModel(const models::studio::EditableStudioModel* model)
{
	if (auto it = _states.find(model); it != _states.end())
	{
		return it.value();
	}

	auto it = _states.insert(model, {});

	return it.value();
}

static ControlPanelState& GetOrCreateStateForPanel(QHash<QString, QSharedPointer<ControlPanelState>>& states, const QString& panelName)
{
	if (auto it = states.find(panelName); it != states.end())
	{
		return *it.value().get();
	}

	return *states.insert(panelName, QSharedPointer<ControlPanelState>(new ControlPanelState())).value().get();
}

void ControlPanels::ClearStateForModel(const models::studio::EditableStudioModel* model)
{
	if (auto it = _states.find(model); it != _states.end())
	{
		_states.erase(it);
	}
}

void ControlPanels::SaveCurrentState(const models::studio::EditableStudioModel* model)
{
	if (model)
	{
		auto& states = GetOrCreateStateForModel(model);

		for (auto controlPanel : _controlPanels)
		{
			auto meta = controlPanel->metaObject();

			auto& state = GetOrCreateStateForPanel(states, meta->className());

			controlPanel->SaveState(state);
		}
	}
}

void ControlPanels::RestoreCurrentState(const models::studio::EditableStudioModel* model)
{
	if (model)
	{
		auto& states = GetOrCreateStateForModel(model);

		for (auto controlPanel : _controlPanels)
		{
			auto meta = controlPanel->metaObject();

			auto& state = GetOrCreateStateForPanel(states, meta->className());

			controlPanel->RestoreState(state);
		}
	}
	else
	{
		//Restore to null state by using an empty state object
		ControlPanelState state;

		for (auto controlPanel : _controlPanels)
		{
			controlPanel->RestoreState(state);
		}
	}
}
}
