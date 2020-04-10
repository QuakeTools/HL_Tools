#pragma once

#include <memory>

#include <QMainWindow>
#include <QSharedPointer>
#include <QString>
#include <QTimer>

#include "ui_HLMVMainWindow.h"

#include "assets/Asset.hpp"

#include "ui/EditorAsset.hpp"

namespace ui
{
class MainPanel;
class ProgramState;
class ViewerWindow;

class HLMVMainWindow final : public QMainWindow
{
	Q_OBJECT

public:
	HLMVMainWindow();
	~HLMVMainWindow();

	ProgramState* GetState() { return _programState; }

private slots:
	void OnTick();

	void PromptLoadAsset();

	void PromptSaveCurrentAsset();

	void OnOpenOptionsDialog();

	void OnCurrentAssetChanged(const QSharedPointer<EditorAsset>& current, const QSharedPointer<EditorAsset>& previous);

	void OnAssetFileNameChanged(const QString& fileName);

	void OnAssetHasUnsavedChangesChanged(bool state);

	void OnTryReloadAsset(const QSharedPointer<EditorAsset>& asset);

	void OnTryCloseAsset(const QSharedPointer<EditorAsset>& asset);

private:
	void UpdateTitle();

	void LoadAsset(const QString& fileName);

	void AddDocumentForAsset(const QString& fileName, std::unique_ptr<Asset>&& asset);

	/**
	*	@brief Save the asset to the filename contained in the asset
	*/
	void SaveAsset(const QSharedPointer<EditorAsset>& asset);

	/**
	*	@brief Prompt the user to save the asset
	*/
	bool PromptSaveAsset(const QSharedPointer<EditorAsset>& asset);

	/**
	*	@brief Ensure that unsaved changes are not unintentionally lost by asking the user to save them
	*/
	bool EnsureNoLostUnsavedChanges(const QSharedPointer<EditorAsset>& asset);

private:
	Ui_HLMVMainWindow _ui;

	ViewerWindow* _viewerWindow;

	QTimer* _timer;

	ProgramState* _programState;
};
}
