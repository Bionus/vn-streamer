#include "window-controller.h"
#include <QThread>
#include <QApplication>
#include <QDesktopWidget>
#include "logger.h"
#include "windows-utils.h"
#include "windows.h"


WindowController::WindowController(HWND window)
	: m_window(window)
{}

QPixmap WindowController::screenshot()
{
	return windowScreenshot(m_window);
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

	LOG(QString("Click (%1, %2, %3)").arg(button == Qt::RightButton ? "right" : "left").arg(pos.x()).arg(pos.y()), Logger::Debug);
}

bool isVKExtended(UINT key)
{
	return (key == VK_INSERT || key == VK_DELETE || key == VK_END || key == VK_DOWN ||
		key == VK_NEXT || key == VK_LEFT || key == VK_RIGHT || key == VK_HOME || key == VK_UP ||
		key == VK_LWIN || key == VK_RWIN || key == VK_RMENU || key == VK_RCONTROL);
}

UINT keyToVK(Qt::Key key)
{
	// 0-9
	if (key >= Qt::Key_0 && key <= Qt::Key_9)
		return 0x30 + (key - Qt::Key_0); // VK_KEY_0

	// a-z
	if (key >= Qt::Key_A && key <= Qt::Key_Z)
		return 0x41 + (key - Qt::Key_A); // VK_KEY_A

	// Function keys
	if (key >= Qt::Key_F1 && key <= Qt::Key_F24)
		return 0x70 + (key - Qt::Key_F1); // VK_F1

	// Mapping for special keys
	static QMap<Qt::Key, UINT> keys = {
		{ Qt::Key_Enter, VK_RETURN },
		{ Qt::Key_Escape, VK_ESCAPE },
		{ Qt::Key_Space, VK_SPACE },
	};

	return keys.value(key, VK_ESCAPE);
}

void WindowController::keyPress(Qt::Key key)
{
	UINT vk = keyToVK(key);

	bool simulate = false;
	if (simulate)
	{
		UINT scan = MapVirtualKey(vk, 0);
		bool extended = isVKExtended(vk);
		LPARAM lparam;

		// Key down
		lparam = 0x00000001 | (LPARAM)(scan << 16);
		if (extended)
			lparam = lparam | 0x01000000;
		SendMessage(m_window, WM_KEYDOWN, vk, lparam);

		// Char
		SendMessage(m_window, WM_CHAR, vk, 0);

		// Key up
		lparam = 0xC0000001 | (LPARAM)(scan << 16);
		if (extended)
			lparam = lparam | 0x01000000;
		SendMessage(m_window, WM_KEYUP, vk, lparam);
	}
	else
	{
		SetForegroundWindow(m_window);

		INPUT input;
		memset(&input, 0, sizeof(INPUT));

		input.type = INPUT_KEYBOARD;
		input.ki.wVk = vk;
		input.ki.dwFlags = KEYEVENTF_SCANCODE;
		input.ki.wScan = MapVirtualKey(vk, 0);

		// Key down
		SendInput(1, &input, sizeof(INPUT));

		QThread::msleep(100);

		// Key up
		input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	LOG(QString("Key press (%1)").arg(key), Logger::Debug);
}
