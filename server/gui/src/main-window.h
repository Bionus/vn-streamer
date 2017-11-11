#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QProcess>
#include "windows.h"
#include "command.h"
#include "profile.h"
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
		void commandReceived(QString client, Command command, QStringList args);

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
		QList<Profile*> m_profiles;
		StreamerServer *m_server;
		TranslationAggregatorController *m_taController;
		QString m_lastText;
		VisualNovelController *m_vnController;
		int m_streaming = 0;
		QMap<QString, HWND> m_windows;
		QMap<HWND, Profile*> m_windowsProfiles;
		QMap<HWND, QString> m_windowsExecutables;
		QTimer *m_windowPoller = Q_NULLPTR;
		QProcess *m_streamingProcess = Q_NULLPTR;
};

#endif // MAIN_WINDOW_H
