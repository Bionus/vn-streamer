#include "window-controller.h"
#include <QThread>
#include <QApplication>
#include <QDesktopWidget>
#include "logger.h"
#include "windows-utils.h"


WindowController::WindowController(HWND window)
	: m_window(window)
{}

QPixmap WindowController::screenshot()
{
	QRect rect = windowRect(m_window);
	SetForegroundWindow(m_window);

	WId desktop = QApplication::desktop()->winId();
	return QPixmap::grabWindow(desktop, rect.x(), rect.y(), rect.width(), rect.height());
}

QRect WindowController::rect()
{
	return windowRect(m_window);
}

void WindowController::click(Qt::MouseButton button, QPoint pos)
{
	if (button != Qt::LeftButton && button != Qt::RightButton)
	{
		LOG(QString("Click with button '%1' is not supported").arg(button), Logger::Error);
		return;
	}

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

		int down = button == Qt::RightButton ? WM_RBUTTONDOWN : WM_LBUTTONDOWN;
		int mk = button == Qt::RightButton ? MK_RBUTTON : MK_LBUTTON;
		int up = button == Qt::RightButton ? WM_RBUTTONUP : WM_LBUTTONUP;

		SendMessage(clickWindow, WM_SETCURSOR, (WPARAM)clickWindow, MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
		SendMessage(clickWindow, WM_MOUSEMOVE, 0, MAKELPARAM(pos.x(), pos.y()));
		SendMessage(clickWindow, WM_MOUSEACTIVATE, (WPARAM)m_window, MAKELPARAM(HTCLIENT, down));
		SendMessage(clickWindow, WM_SETFOCUS, (WPARAM)m_window, 0);
		SendMessage(clickWindow, WM_SETCURSOR, (WPARAM)clickWindow, MAKELPARAM(HTCLIENT, down));
		SendMessage(clickWindow, down, mk, MAKELPARAM(pos.x(), pos.y()));
		SendMessage(clickWindow, WM_MOUSEMOVE, 0, MAKELPARAM(pos.x(), pos.y()));
		SendMessage(clickWindow, WM_SETCURSOR, (WPARAM)clickWindow, MAKELPARAM(HTCLIENT, up));
		SendMessage(clickWindow, up, 0, MAKELPARAM(pos.x(), pos.y()));
	}
	else
	{
		INPUT input;
		SetCursorPos(screenCoords.x(), screenCoords.y());

		int down = button == Qt::RightButton ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_LEFTDOWN;
		int up = button == Qt::RightButton ? MOUSEEVENTF_RIGHTUP : MOUSEEVENTF_LEFTUP;

		memset(&input, 0, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = down;
		SendInput(1, &input, sizeof(INPUT));

		QThread::msleep(100);

		memset(&input, 0, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = up;
		SendInput(1, &input, sizeof(INPUT));
	}

	LOG(QString("Click (%1, %3, %4)").arg(button == Qt::RightButton ? "right" : "left").arg(pos.x()).arg(pos.y()), Logger::Debug);
}
