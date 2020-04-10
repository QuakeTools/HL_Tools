#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>

#include "assets/Asset.hpp"
#include "assets/AssetIO.hpp"
#include "assets/AssetTypes.hpp"

#include "assets/models/generic_mesh/GenericMeshCollector.hpp"

#include "assets/models/studio/EditableStudioModel.hpp"
#include "assets/models/studio/StudioModelAssetReader.hpp"
#include "assets/models/studio/StudioModelAssetWriter.hpp"

#include "game/World.hpp"
#include "game/components/Entities.hpp"
#include "game/systems/TransformSystem.hpp"

#include "graphics/Scene.hpp"

#include "graphics/components/Camera.hpp"

#include "ui/CameraOperators.hpp"

#include "ui/HLMVMainWindow.hpp"
#include "ui/ProgramState.hpp"
#include "ui/ViewerWindow.hpp"

#include "ui/dialogs/SaveAssetDialog.hpp"

#include "ui/dockpanels/AssetExplorer.hpp"
#include "ui/dockpanels/CameraInfo.hpp"
#include "ui/dockpanels/DisplayProperties.hpp"
#include "ui/dockpanels/RenderProperties.hpp"
#include "ui/dockpanels/SceneInfo.hpp"
#include "ui/dockpanels/Timeline.hpp"

#include "ui/options/OptionsDialog.hpp"

namespace ui
{
const QString FileDialogAssetFilter{QStringLiteral("Half-Life 1 Model Files (*.mdl *.spr *.bsp *.dol);;All Files (*.*)")};

HLMVMainWindow::HLMVMainWindow()
	: QMainWindow()
	, _timer(new QTimer(this))
	, _programState(new ProgramState(this))
	, _cameraOperator(std::make_unique<FreeLookCameraOperator>())
{
	{
		//TODO: automate type registration
		auto assetTypes = _programState->GetAssetTypes();

		assetTypes->Add(std::make_unique<AssetType>(
			entt::type_info<models::studio::EditableStudioModel>::id(),
			std::make_unique<models::studio::StudioModelAssetReader>(),
			std::make_unique<models::studio::StudioModelAssetWriter>()));
	}

	_ui.setupUi(this);

	//The main window should be deleted automatically
	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose, true);

	_viewerWindow = new ViewerWindow(this);

	const auto viewerWindowWidget = _viewerWindow->GetContainer();

	viewerWindowWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	setCentralWidget(viewerWindowWidget);

	auto createDock = [this](QWidget* widget, const QString& title, const Qt::DockWidgetAreas areas, const Qt::DockWidgetArea initialArea)
	{
		auto dock = new QDockWidget(title, this);

		dock->setAllowedAreas(areas);

		dock->setWidget(widget);

		addDockWidget(initialArea, dock);
	};

	createDock(new dockpanels::AssetExplorer(_programState->GetAssets(), this), "Asset Explorer",
		Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea | Qt::DockWidgetArea::BottomDockWidgetArea, Qt::DockWidgetArea::LeftDockWidgetArea);

	createDock(new dockpanels::DisplayProperties(this), "Display Properties",
		Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea | Qt::DockWidgetArea::BottomDockWidgetArea, Qt::DockWidgetArea::RightDockWidgetArea);

	createDock(new dockpanels::RenderProperties(this), "Render Properties",
		Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea | Qt::DockWidgetArea::BottomDockWidgetArea, Qt::DockWidgetArea::RightDockWidgetArea);

	createDock(new dockpanels::SceneInfo(this), "Scene Info",
		Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea | Qt::DockWidgetArea::BottomDockWidgetArea, Qt::DockWidgetArea::LeftDockWidgetArea);

	auto cameraInfo = new dockpanels::CameraInfo(this);

	createDock(cameraInfo, "Camera Info",
		Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea | Qt::DockWidgetArea::BottomDockWidgetArea, Qt::DockWidgetArea::LeftDockWidgetArea);

	connect(GetState(), &ProgramState::CameraInfoChanged, cameraInfo, &dockpanels::CameraInfo::UpdateCameraInfo);

	createDock(new dockpanels::Timeline(this), "Timeline",
		Qt::DockWidgetArea::BottomDockWidgetArea, Qt::DockWidgetArea::BottomDockWidgetArea);

	connect(_timer, &QTimer::timeout, this, &HLMVMainWindow::OnTick);

	connect(_ui.ActionLoad, &QAction::triggered, this, &HLMVMainWindow::PromptLoadAsset);
	connect(_ui.ActionSave, &QAction::triggered, this, &HLMVMainWindow::PromptSaveCurrentAsset);
	connect(_ui.ActionOptions, &QAction::triggered, this, &HLMVMainWindow::OnOpenOptionsDialog);

	{
		auto assets = GetState()->GetAssets();

		connect(assets, &EditorAssetList::CurrentAssetChanged, this, &HLMVMainWindow::OnCurrentAssetChanged);
		connect(assets, &EditorAssetList::ReloadAsset, this, &HLMVMainWindow::OnTryReloadAsset);
		connect(assets, &EditorAssetList::CloseAsset, this, &HLMVMainWindow::OnTryCloseAsset);
	}

	connect(_viewerWindow, &ViewerWindow::MouseEvent, this, &HLMVMainWindow::OnViewerWindowMouseEvent);

	_timer->start(0);
}

HLMVMainWindow::~HLMVMainWindow() = default;

void HLMVMainWindow::OnTick()
{
	auto currentAsset = GetState()->GetAssets()->GetCurrentAsset();

	//TODO: update frequency should be controllable
	if (currentAsset)
	{
		currentAsset->GetWorld()->Update();
	}

	_viewerWindow->update();

	//Update the camera after a frame so any pending changes to camera settings are flushed to all components
	if (_cameraUpdated)
	{
		_cameraUpdated = false;

		emit GetState()->CameraInfoChanged(currentAsset);
	}
}

void HLMVMainWindow::PromptLoadAsset()
{
	if (const auto fileName = QFileDialog::getOpenFileName(this, "Select asset", {}, FileDialogAssetFilter);
		!fileName.isEmpty())
	{
		LoadAsset(fileName);
	}
}

void HLMVMainWindow::PromptSaveCurrentAsset()
{
	if (const auto asset = GetState()->GetAssets()->GetCurrentAsset(); asset && asset->HasUnsavedChanges())
	{
		PromptSaveAsset(asset);
	}
}

void HLMVMainWindow::OnOpenOptionsDialog()
{
	options::OptionsDialog dialog(this);

	dialog.exec();
}

void HLMVMainWindow::OnCurrentAssetChanged(const QSharedPointer<EditorAsset>& current, const QSharedPointer<EditorAsset>& previous)
{
	if (previous)
	{
		disconnect(previous.get(), &EditorAsset::FileNameChanged, this, &HLMVMainWindow::OnAssetFileNameChanged);
		disconnect(previous.get(), &EditorAsset::UnsavedChangesChanged, this, &HLMVMainWindow::OnAssetHasUnsavedChangesChanged);
	}

	game::World* world = nullptr;

	if (current)
	{
		world = current->GetWorld();

		connect(current.get(), &EditorAsset::FileNameChanged, this, &HLMVMainWindow::OnAssetFileNameChanged);
		connect(current.get(), &EditorAsset::UnsavedChangesChanged, this, &HLMVMainWindow::OnAssetHasUnsavedChangesChanged);
	}

	_viewerWindow->SetWorld(world);

	UpdateTitle();

	_cameraUpdated = true;
}

void HLMVMainWindow::OnAssetFileNameChanged(const QString& fileName)
{
	UpdateTitle();
}

void HLMVMainWindow::OnAssetHasUnsavedChangesChanged(bool state)
{
	UpdateTitle();
}

void HLMVMainWindow::OnTryReloadAsset(const QSharedPointer<EditorAsset>& asset)
{
	if (!EnsureNoLostUnsavedChanges(asset))
	{
		return;
	}

	//TODO: use LoadAsset to reload the asset and update the required data in asset
}

void HLMVMainWindow::OnTryCloseAsset(const QSharedPointer<EditorAsset>& asset)
{
	if (!EnsureNoLostUnsavedChanges(asset))
	{
		return;
	}

	GetState()->GetAssets()->RemoveAsset(asset);
}

void HLMVMainWindow::OnViewerWindowMouseEvent(QMouseEvent& event)
{
	if (const auto asset = GetState()->GetAssets()->GetCurrentAsset(); asset && asset->GetCurrentCamera() != entt::null)
	{
		_cameraOperator->MouseEvent(*asset->GetWorld()->GetRegistry(), asset->GetCurrentCamera(), event);

		_cameraUpdated = true;

		//emit GetState()->CameraInfoChanged(GetState()->GetAssets()->GetCurrentAsset());
	}
}

void HLMVMainWindow::UpdateTitle()
{
	QString title;

	bool modified = false;

	if (const auto asset = GetState()->GetAssets()->GetCurrentAsset(); asset)
	{
		title = asset->GetFileName() + "[*]";
		modified = asset->HasUnsavedChanges();
	}

	setWindowTitle(title);
	setWindowModified(modified);
}

void HLMVMainWindow::LoadAsset(const QString& fileName)
{
	//TODO: support loading asset "in place" in an existing asset for asset reloading
	try
	{
		const auto stdFileName = fileName.toStdString();

		auto asset = ReadAsset(*GetState()->GetAssetTypes(), stdFileName);

		AddDocumentForAsset(fileName, std::move(asset));
	}
	catch (const AssetException& e)
	{
		QMessageBox::critical(this, "Error loading asset", QString("Error loading asset \"%1\":\n%2").arg(fileName, e.what()));
	}
}

void HLMVMainWindow::AddDocumentForAsset(const QString& fileName, std::unique_ptr<Asset>&& asset)
{
	QSharedPointer<EditorAsset> editorAsset{new EditorAsset(fileName, std::move(asset))};

	//TODO: automate system hookup somehow
	//TODO: shouldn't be in this class since it's in a studio model specific class
	auto& world = *editorAsset->GetWorld();

	{
		world.UpdateSink.connect<&game::systems::UpdateRotations>();
		world.UpdateSink.connect<&game::systems::UpdateLocalToWorld>();
		world.UpdateSink.connect<&game::systems::UpdateLocalToParent>();
	}

	{
		auto& registry = *world.GetRegistry();

		auto& scene = registry.set<graphics::Scene>();

		scene.CollectObjectsSink.connect<&models::generic_mesh::CollectGenericMesh>();

		//Create the default camera
		//TODO: use functions to create interdependent components at the same time
		auto cameraEntity = registry.create();

		auto& camera = registry.assign<graphics::components::Camera>(cameraEntity);

		camera.BackgroundColor = glm::vec4{63 / 255.f, 127 / 255.f, 127 / 255.f, 1.f};

		auto& position = registry.assign<game::components::Translation>(cameraEntity);
		auto& rotationXYZ = registry.assign<game::components::RotationEulerXYZ>(cameraEntity);
		auto& rotation = registry.assign<game::components::Rotation>(cameraEntity);

		position.Value.z = 10.f;

		auto& localToWorld = registry.assign<game::components::LocalToWorld>(cameraEntity);

		editorAsset->SetCurrentCamera(cameraEntity);
	}

	//TODO: add collect handler for studio model
	GetState()->GetAssets()->AddAsset(editorAsset);

	GetState()->GetAssets()->SetCurrentAsset(editorAsset);
}

void HLMVMainWindow::SaveAsset(const QSharedPointer<EditorAsset>& asset)
{
	//TODO: handle saving
	asset->SetHasUnsavedChanges(false);
}

bool HLMVMainWindow::PromptSaveAsset(const QSharedPointer<EditorAsset>& asset)
{
	dialogs::SaveAssetDialog dlg{asset->GetFileName(), FileDialogAssetFilter, this};

	if (dlg.exec() == QDialog::DialogCode::Accepted)
	{
		asset->SetFileName(dlg.GetFileName());

		SaveAsset(asset);

		return true;
	}

	return false;
}

bool HLMVMainWindow::EnsureNoLostUnsavedChanges(const QSharedPointer<EditorAsset>& asset)
{
	if (!asset->HasUnsavedChanges())
	{
		return true;
	}

	const auto action = QMessageBox::question(this,
		"The asset has been modified", "Do you want to save your changes?",
		QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::Discard | QMessageBox::StandardButton::Cancel,
		QMessageBox::StandardButton::Yes);

	switch (action)
	{
	case QMessageBox::StandardButton::Yes:
		break;

		//If the user wants to discard changes then we can just continue right away
	case QMessageBox::StandardButton::Discard:
		return true;

		//Stop whatever we were trying to do
	case QMessageBox::StandardButton::Cancel:
		return false;

	default: return false;
	}

	return PromptSaveAsset(asset);
}
}
