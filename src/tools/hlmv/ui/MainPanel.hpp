#pragma once

#include <memory>

#include <QObject>
#include <QString>
#include <QTimer>
#include <QWidget>

#include "ui_MainPanel.h"

#include "game/World.hpp"

#include "models/studio/EditableStudioModel.hpp"

namespace ui
{
class ControlPanels;
class ViewerWindow;

/**
*	@brief Stores the information of a single model
*/
class ModelDocument final : public QObject
{
	Q_OBJECT

public:
	ModelDocument(std::unique_ptr<models::studio::EditableStudioModel>&& model, std::unique_ptr<game::World>&& world)
		: Model(std::move(model))
		, World(std::move(world))
	{
	}

	~ModelDocument();

	std::unique_ptr<models::studio::EditableStudioModel> Model;
	std::unique_ptr<game::World> World;
};

class MainPanel final : public QWidget
{
	Q_OBJECT

public:
	MainPanel(QWidget* parent = nullptr);
	~MainPanel();

	void PromptLoadModel();

signals:
	void ModelChanged(models::studio::EditableStudioModel* model);

	void ModelRemoved(models::studio::EditableStudioModel* model);

private slots:
	void OnTick();

	void OnToggleControlPanels(bool checked);

	void OnToggleMainControls(bool checked);

	void OnModelChanged(int index);

	void OnCloseModel();

private:
	void AddDocumentForModel(const QString& fileName, std::unique_ptr<models::studio::EditableStudioModel>&& model);

private:
	Ui_MainPanel _ui;

	ViewerWindow* _viewerWindow;

	QTimer* _timer;
};
}
