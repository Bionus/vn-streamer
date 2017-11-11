#ifndef VISUAL_NOVEL_CONTROLLER_H
#define VISUAL_NOVEL_CONTROLLER_H

#include <QObject>
#include <QPixmap>
#include <QSettings>
#include "window-controller.h"
#include "profile.h"


class VisualNovelController : public QObject
{
	Q_OBJECT

	public:
		explicit VisualNovelController(WindowController *window, QSettings *settings, Profile *profile = Q_NULLPTR, QObject *parent = Q_NULLPTR);
		QPixmap getImage();

	public slots:
		void next();
		void hideTextbox();
		void showTextbox();

	protected:
		void executeCommand(QString command);

	private:
		WindowController *m_window;
		QSettings *m_settings;
		Profile *m_profile;
};

#endif // VISUAL_NOVEL_CONTROLLER_H
