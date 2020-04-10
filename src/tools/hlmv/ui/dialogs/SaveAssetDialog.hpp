#pragma once

#include <QDialog>
#include <QFileDialog>

#include "ui_SaveAssetDialog.h"

namespace ui::dialogs
{
class SaveAssetDialog final : public QDialog
{
public:
	SaveAssetDialog(const QString& fileName, const QString& filter, QWidget* parent = nullptr)
		: QDialog(parent)
		, _filter(filter)
	{
		_ui.setupUi(this);

		_ui.FileName->setText(fileName);

		connect(_ui.BrowseFileName, &QPushButton::clicked, this, &SaveAssetDialog::OnBrowseFileName);
	}

	~SaveAssetDialog() = default;

	QString GetFileName() const
	{
		return _ui.FileName->text();
	}

private slots:
	void OnBrowseFileName()
	{
		const auto newFileName = QFileDialog::getSaveFileName(this, "Select filename", _ui.FileName->text(), _filter);

		if (!newFileName.isEmpty())
		{
			_ui.FileName->setText(newFileName);
		}
	}

private:
	Ui_SaveAssetDialog _ui;

	const QString _filter;
};
}
