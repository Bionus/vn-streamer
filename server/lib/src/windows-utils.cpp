#include "windows-utils.h"
#include <QtWinExtras/QtWin>
#include "psapi.h"


BOOL CALLBACK enumWindow(HWND hwnd, LPARAM lParam)
{
	if (!IsWindowVisible(hwnd))
		return TRUE;

	QString title = windowTitle(hwnd);
	QMap<QString, HWND>& titles = *reinterpret_cast<QMap<QString, HWND>*>(lParam);
	titles.insert(title, hwnd);

	return TRUE;
}

QMap<QString, HWND> getAllWindows()
{
	QMap<QString, HWND> ret;
	EnumWindows(enumWindow, reinterpret_cast<LPARAM>(&ret));
	return ret;
}

QString getWindowExecutable(HWND window)
{
	DWORD processId;
	GetWindowThreadProcessId(window, &processId);
	HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

	const DWORD BUFFER_SIZE = 1024;
	wchar_t *ch = new wchar_t[BUFFER_SIZE];
	GetModuleFileNameExW(process, NULL, ch, BUFFER_SIZE);

	return QString::fromWCharArray(ch);
}

HWND getWindow(QString name)
{
	LPCSTR title = name.toStdString().c_str();
	return FindWindow(NULL, title);
}

QRect windowRect(HWND window)
{
	RECT rect;
	POINT topLeft = { 0, 0 };
	if (window == NULL || !GetClientRect(window, &rect) || !ClientToScreen(window, &topLeft))
		return QRect();

	return QRect(topLeft.x, topLeft.y, rect.right, rect.bottom);
}

QString windowTitle(HWND window)
{
	if (window == NULL)
		return QString();

	const DWORD BUFFER_SIZE = 1024;
	wchar_t *ch = new wchar_t[BUFFER_SIZE];
	GetWindowTextW(window, ch, BUFFER_SIZE);
	return QString::fromWCharArray(ch);
}

QString windowText(HWND window)
{
	if (window == NULL)
		return QString();

	const DWORD BUFFER_SIZE = 4096;
	wchar_t *ch = new wchar_t[BUFFER_SIZE];
	SendMessageW(window, WM_GETTEXT, (WPARAM)BUFFER_SIZE, (LPARAM)ch);
	return QString::fromWCharArray(ch);
}

QPixmap windowScreenshot(HWND window)
{
	QRect rect = windowRect(window);

	HDC hdcSource = GetDC(window);
	HDC hdcCapture = CreateCompatibleDC(hdcSource);

	HBITMAP captureBitmap = CreateCompatibleBitmap(hdcSource, rect.width(), rect.height());
	SelectObject(hdcCapture, captureBitmap);
	BitBlt(hdcCapture, 0, 0, rect.width(), rect.height(), hdcSource, 0, 0, SRCCOPY | CAPTUREBLT);

	QPixmap res = QtWin::fromHBITMAP(captureBitmap);

	ReleaseDC(window, hdcSource);
	DeleteDC(hdcCapture);
	DeleteObject(captureBitmap);

	return res;
}
