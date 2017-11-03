#include "main-window.h"
#include <QApplication>
#include "logger.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("VN Streamer");
	app.setApplicationDisplayName("VN Streamer");
	app.setApplicationVersion(VERSION);
	app.setOrganizationName("Bionus");
	app.setOrganizationDomain("bionus.fr.cr");

	Logger::getInstance().setLogLevel(Logger::Debug);

	MainWindow mainWindow;
	mainWindow.show();

	return app.exec();
}
