#ifndef WINDOW_CONTROLLER_H
#define WINDOW_CONTROLLER_H

#include <QObject>
#include <QList>
#include <QPixmap>
#include "windows.h"


class WindowController : public QObject
{
	Q_OBJECT

	public:
		explicit WindowController(HWND window);
		QPixmap screenshot();
		QRect rect();

	public slots:
		void click(Qt::MouseButton button, QPoint pos);
		void keyPress(Qt::Key key);

	private:
		HWND m_window;
};

#endif // WINDOW_CONTROLLER_H
