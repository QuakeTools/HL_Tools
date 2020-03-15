#pragma once

#include <QLocalServer>
#include <QMainWindow>
#include <QObject>
#include <QScopedPointer>

namespace ui
{
class HLMVMainWindow;
}

/**
*	@brief Handles program startup and shutdown
*/
class HLMVApplication final : public QObject
{
	Q_OBJECT

public:
	HLMVApplication() = default;
	~HLMVApplication() = default;

	int Run(int argc, char* argv[]);

private slots:
	void OnExit();

	void OnNewConnection();

private:
	ui::HLMVMainWindow* _mainWindow;

	//Used to manage single-instance functionality
	QScopedPointer<QLocalServer> _server;
};
