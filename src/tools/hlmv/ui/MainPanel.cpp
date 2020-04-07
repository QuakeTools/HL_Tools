#include <QBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSharedPointer>

#include "game/World.hpp"
#include "game/components/Entities.hpp"
#include "game/systems/TransformSystem.hpp"

#include "graphics/Scene.hpp"

#include "graphics/components/Camera.hpp"

#include "models/generic_mesh/GenericMeshCollector.hpp"

#include "models/studio/EditableStudioModel.hpp"
#include "models/studio/StudioFileFormat.hpp"
#include "models/studio/StudioIO.hpp"

#include "ui/ControlPanels.hpp"
#include "ui/MainPanel.hpp"
#include "ui/ViewerWindow.hpp"

namespace ui
{
ModelDocument::~ModelDocument() = default;

MainPanel::MainPanel(QWidget* parent)
	: QWidget(parent)
{
	_ui.setupUi(this);

	_viewerWindow = new ViewerWindow(this);

	const auto viewerWindowWidget = _viewerWindow->GetContainer();

	viewerWindowWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	_timer = new QTimer(this);

	_ui.ModelsDocuments->setEnabled(false);
	_ui.ModelCloseButton->setEnabled(false);

	connect(_timer, &QTimer::timeout, this, &MainPanel::OnTick);

	connect(_ui.MinimizeControlPanelsToggle, &QToolButton::toggled, this, &MainPanel::OnToggleControlPanels);
	connect(_ui.MainControlsToggle, &QToolButton::toggled, this, &MainPanel::OnToggleMainControls);

	connect(_ui.ModelsDocuments, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainPanel::OnModelChanged);
	connect(_ui.ModelCloseButton, &QPushButton::pressed, this, &MainPanel::OnCloseModel);

	connect(this, &MainPanel::ModelChanged, _ui.ControlPanels, &ControlPanels::OnModelChanged);
	connect(this, &MainPanel::ModelRemoved, _ui.ControlPanels, &ControlPanels::OnModelRemoved);

	_ui.MainLayout->insertWidget(0, viewerWindowWidget, 2);

	_timer->start(0);
}

MainPanel::~MainPanel() = default;

void MainPanel::PromptLoadModel()
{
	if (const auto fileName = QFileDialog::getOpenFileName(this, "Select model", {}, "Studio model files (*.mdl *.dol);;All files (*.*)");
		!fileName.isEmpty())
	{
		try
		{
			const auto stdFileName = fileName.toStdString();

			auto data = models::studio::LoadStudioFiles(stdFileName);

			auto editable = models::studio::ConvertToEditable(stdFileName, data);

			AddDocumentForModel(fileName, std::make_unique<models::studio::EditableStudioModel>(std::move(editable)));
		}
		catch (const models::studio::StudioModelException& e)
		{
			QMessageBox::critical(this, "Error loading studio model", QString("Error loading studio model \"%1\":\n%2").arg(fileName, e.what()));
		}
	}
}

void MainPanel::AddDocumentForModel(const QString& fileName, std::unique_ptr<models::studio::EditableStudioModel>&& model)
{
	const QFileInfo fileInfo{fileName};

	//TODO: automate system hookup somehow
	//TODO: shouldn't be in this class since it's in a studio model specific class
	auto world = std::make_unique<game::World>();

	{
		world->UpdateSink.connect<&game::systems::UpdateRotations>();
		world->UpdateSink.connect<&game::systems::UpdateLocalToWorld>();
		world->UpdateSink.connect<&game::systems::UpdateLocalToParent>();
	}

	{
		auto& registry = world->GetRegistry();

		auto& scene = registry.set<graphics::Scene>();

		scene.CollectObjectsSink.connect<&models::generic_mesh::CollectGenericMesh>();

		//Create the default camera
		auto cameraEntity = registry.create();

		auto& camera = registry.assign<graphics::components::Camera>(cameraEntity);

		camera.BackgroundColor = glm::vec3{63 / 255.f, 127 / 255.f, 127 / 255.f};

		auto& localToWorld = registry.assign<game::components::LocalToWorld>(cameraEntity);

		auto& position = registry.assign<game::components::Translation>(cameraEntity);

		position.Value.z = 10.f;
	}

	//TODO: add collect handler for studio model

	_ui.ModelsDocuments->addItem(fileInfo.fileName(),
		QVariant::fromValue(QSharedPointer<ModelDocument>
		{
			new ModelDocument(std::move(model), std::move(world)),
		}
	));

	const auto index = _ui.ModelsDocuments->count() - 1;

	//If this is the first model to be opened, enable the list and the close button
	if (_ui.ModelsDocuments->count() == 1)
	{
		_ui.ModelsDocuments->setEnabled(true);
		_ui.ModelCloseButton->setEnabled(true);
	}

	_ui.ModelsDocuments->setCurrentIndex(index);
}

void MainPanel::OnTick()
{
	//TODO: rework timer logic
	_viewerWindow->update();
}

void MainPanel::OnToggleControlPanels(bool checked)
{
	_ui.ControlPanelsLayoutWidget->setVisible(!checked);

	const auto arrowType = checked ? Qt::ArrowType::UpArrow : Qt::ArrowType::DownArrow;

	_ui.MinimizeControlPanelsToggle->setArrowType(arrowType);
}

void MainPanel::OnToggleMainControls(bool checked)
{
	_ui.MainControls->setVisible(!checked);

	const auto arrowType = checked ? Qt::ArrowType::RightArrow : Qt::ArrowType::LeftArrow;

	_ui.MainControlsToggle->setArrowType(arrowType);
}

void MainPanel::OnModelChanged(int index)
{
	models::studio::EditableStudioModel* model = nullptr;
	game::World* world = nullptr;

	if (index != -1)
	{
		auto document = _ui.ModelsDocuments->itemData(index).value<QSharedPointer<ModelDocument>>();

		model = document->Model.get();
		world = document->World.get();
	}

	emit ModelChanged(model);

	_viewerWindow->SetWorld(world);
}

void MainPanel::OnCloseModel()
{
	const auto index = _ui.ModelsDocuments->currentIndex();

	if (index != -1)
	{
		auto model = _ui.ModelsDocuments->itemData(index).value<QSharedPointer<ModelDocument>>()->Model.get();

		emit ModelRemoved(model);

		_ui.ModelsDocuments->removeItem(index);
	}

	if (_ui.ModelsDocuments->count() == 0)
	{
		_ui.ModelsDocuments->setEnabled(false);
		_ui.ModelCloseButton->setEnabled(false);
	}
}
}
