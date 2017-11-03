#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QProcess>
#include "windows.h"
#include "command.h"
#include "streamer-server.h"
#include "translation-aggregator-controller.h"
#include "visual-novel-controller.h"


namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = Q_NULLPTR);
		~MainWindow();
		void closeEvent(QCloseEvent *event) override;

	public slots:
		void pollWindows();
		void dataChanged();
		void newLog(QString message);
		void commandReceived(Command command, QStringList args);

		// Server controls
		void toggle();
		void start();
		void stop();

		// Menu
		void fileSettings();
		void aboutProgram();
		void aboutGithub();
		void aboutReportBug();

	private:
		Ui::MainWindow *ui;
		QSettings *m_settings;
		StreamerServer *m_server;
		TranslationAggregatorController *m_taController;
		QString m_lastText;
		VisualNovelController *m_vnController;
		int m_streaming = 0;
		QMap<QString, HWND> m_windows;
		QTimer *m_windowPoller = Q_NULLPTR;
		QProcess *m_streamingProcess = Q_NULLPTR;
};

#endif // MAIN_WINDOW_H
