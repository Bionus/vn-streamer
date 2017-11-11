#ifndef WINDOWS_UTILS_H
#define WINDOWS_UTILS_H

#include "windows.h"
#include <QMap>
#include <QString>
#include <QRect>
#include <QPixmap>


QMap<QString, HWND> getAllWindows();
QString getWindowExecutable(HWND window);
HWND getWindow(QString name);
QRect windowRect(HWND window);
QString windowTitle(HWND window);
QString windowText(HWND window);
QPixmap windowScreenshot(HWND window);

#endif // WINDOWS_UTILS_H
