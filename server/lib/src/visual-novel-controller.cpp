#include "visual-novel-controller.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include "windows-utils.h"
#include "logger.h"


VisualNovelController::VisualNovelController(HWND window, QSettings *settings, QObject *parent)
	: QObject(parent), m_window(window), m_settings(settings)
{}

QPixmap VisualNovelController::getImage()
{
	QRect rect = windowRect(m_window);
	SetForegroundWindow(m_window);

	bool hideTextBox = m_settings->value("hideTextbox", true).toBool();
	if (hideTextBox)
		toggleTextBox();

	QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(), rect.x(), rect.y(), rect.width(), rect.height());

	if (hideTextBox)
		QTimer::singleShot(500, this, &VisualNovelController::toggleTextBox);

	return pixmap;
}

#include <QThread>
void VisualNovelController::click(QPoint pos, bool right)
{
	QRect rect = windowRect(m_window);
	QPoint screenCoords(rect.left() + pos.x(), rect.top() + pos.y());
	SetForegroundWindow(m_window);

	bool simulate = false;
	if (simulate)
	{
		HWND clickWindow;
		HWND child = m_window;
		POINT win_coords;
		do
		{
			clickWindow = child;
			win_coords = { screenCoords.x(), screenCoords.y() };
			ScreenToClient(clickWindow, &win_coords);
			child = ChildWindowFromPoint(clickWindow, win_coords);
		} while (child != NULL && child != clickWindow);

		if (clickWindow == NULL)
		{
			LOG("Could not find the click window", Logger::Error);
			return;
		}

		int down = right ? WM_RBUTTONDOWN : WM_LBUTTONDOWN;
		int up = right ? WM_RBUTTONUP : WM_LBUTTONUP;

		SendMessage(clickWindow, WM_SETCURSOR, (WPARAM)clickWindow, MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
		SendMessage(clickWindow, WM_MOUSEMOVE, 0, MAKELPARAM(pos.x(), pos.y()));
		SendMessage(clickWindow, WM_MOUSEACTIVATE, (WPARAM)m_window, MAKELPARAM(HTCLIENT, down));
		SendMessage(clickWindow, WM_SETFOCUS, (WPARAM)m_window, 0);
		SendMessage(clickWindow, WM_SETCURSOR, (WPARAM)clickWindow, MAKELPARAM(HTCLIENT, down));
		SendMessage(clickWindow, down, right ? MK_RBUTTON : MK_LBUTTON, MAKELPARAM(pos.x(), pos.y()));
		SendMessage(clickWindow, WM_MOUSEMOVE, 0, MAKELPARAM(pos.x(), pos.y()));
		SendMessage(clickWindow, WM_SETCURSOR, (WPARAM)clickWindow, MAKELPARAM(HTCLIENT, up));
		SendMessage(clickWindow, up, 0, MAKELPARAM(pos.x(), pos.y()));
	}
	else
	{
		INPUT input;
		SetCursorPos(screenCoords.x(), screenCoords.y());

		memset(&input, 0, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = right ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(INPUT));

		QThread::msleep(100);

		memset(&input, 0, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = right ? MOUSEEVENTF_RIGHTUP : MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	LOG(QString("Click (%1, %3, %4)").arg(right ? "right" : "left").arg(pos.x()).arg(pos.y()), Logger::Debug);
}

void VisualNovelController::next()
{
	click(QPoint(50, 50), false);
}

void VisualNovelController::toggleTextBox()
{
	click(QPoint(100, 100), true);
}
