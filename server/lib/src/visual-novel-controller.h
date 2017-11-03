#ifndef VISUAL_NOVEL_CONTROLLER_H
#define VISUAL_NOVEL_CONTROLLER_H

#include <QObject>
#include <QPixmap>
#include <QSettings>
#include "windows.h"


class VisualNovelController : public QObject
{
	Q_OBJECT

	public:
		explicit VisualNovelController(HWND window, QSettings *settings, QObject *parent = Q_NULLPTR);
		QPixmap getImage();

	public slots:
		void next();
		void toggleTextBox();

	protected:
		void click(QPoint pos, bool right = false);

	private:
		HWND m_window;
		QSettings *m_settings;
};

#endif // VISUAL_NOVEL_CONTROLLER_H
