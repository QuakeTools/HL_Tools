#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QLocalServer>
#include <QLocalSocket>
#include <QMessageBox>
#include <QScopedPointer>
#include <QSettings>
#include <QStyleFactory>
#include <QSurfaceFormat>

#include "application/HLMVApplication.h"
#include "ui/HLMVMainWindow.h"

int HLMVApplication::Run(int argc, char* argv[])
{
	const QString programName{"Half-Life Model Viewer"};

	QApplication::setOrganizationName(programName);
	QApplication::setOrganizationDomain("https://github.com/Solokiller/HL_Tools");
	QApplication::setApplicationName(programName);
	QApplication::setApplicationDisplayName(programName);

	QSettings::setDefaultFormat(QSettings::Format::IniFormat);

	QApplication::setAttribute(Qt::ApplicationAttribute::AA_ShareOpenGLContexts, true);

	//Set up the OpenGL surface settings to match the Half-Life engine's requirements
	//Vanilla Half-Life uses OpenGL 1.0 for game rendering
	//TODO: eventually an option should be added to allow switching to 3.3 for shader based rendering
	{
		QSurfaceFormat defaultFormat(QSurfaceFormat::FormatOption::DebugContext | QSurfaceFormat::FormatOption::DeprecatedFunctions);

		defaultFormat.setMajorVersion(1);
		defaultFormat.setMinorVersion(0);
		defaultFormat.setProfile(QSurfaceFormat::OpenGLContextProfile::NoProfile);

		defaultFormat.setDepthBufferSize(24);
		defaultFormat.setStencilBufferSize(8);
		defaultFormat.setSwapBehavior(QSurfaceFormat::SwapBehavior::DoubleBuffer);
		defaultFormat.setRedBufferSize(4);
		defaultFormat.setGreenBufferSize(4);
		defaultFormat.setBlueBufferSize(4);
		defaultFormat.setAlphaBufferSize(0);

		QSurfaceFormat::setDefaultFormat(defaultFormat);
	}

	QApplication app(argc, argv);

	connect(&app, &QApplication::aboutToQuit, this, &HLMVApplication::OnExit);

	QSettings settings;

	//TODO: load settings
	//TODO: improve settings loading to avoid having it be this messy
	settings.beginGroup("startup");
	const auto useSingleInstance = settings.value("useSingleInstance", false).toBool();
	settings.endGroup();

	QString fileName;

	{
		QCommandLineParser parser;

		parser.addPositionalArgument("fileName", "Filename of the model to load on startup", "[fileName]");

		parser.process(app);

		const auto positionalArguments = parser.positionalArguments();

		if (!positionalArguments.empty())
		{
			fileName = positionalArguments[0];
		}
	}

	if (useSingleInstance)
	{
		//Check if an instance is already running
		QLocalSocket socket;

		socket.connectToServer(programName, QIODevice::OpenModeFlag::WriteOnly);

		if (socket.waitForConnected(1000))
		{
			//If a filename was specified, this will open the file in the existing instance
			//Otherwise, the existing instance can choose to set focus on itself
			socket.write(fileName.toStdString().c_str());

			socket.waitForBytesWritten(-1);
			socket.waitForDisconnected(-1);

			return 0;
		}
	}

	if (useSingleInstance)
	{
		_server.reset(new QLocalServer());

		connect(_server.get(), &QLocalServer::newConnection, this, &HLMVApplication::OnNewConnection);

		_server->listen(programName);
	}

	_mainWindow = new ui::HLMVMainWindow();

	_mainWindow->showMaximized();

	//TODO: open file specified on command line

	return app.exec();
}

void HLMVApplication::OnExit()
{
	_mainWindow = nullptr;

	if (_server)
	{
		_server->close();
		_server.reset();
	}
}

void HLMVApplication::OnNewConnection()
{
	auto socket = _server->nextPendingConnection();

	if (socket->waitForReadyRead(1000))
	{
		const auto buffer = socket->readAll();

		const QString fileName{buffer};

		//TODO: load model
	}

	socket->close();
}
