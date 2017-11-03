#ifndef WINDOWS_UTILS_H
#define WINDOWS_UTILS_H

#include "windows.h"
#include <QMap>
#include <QString>
#include <QRect>


QMap<QString, HWND> getAllWindows();
HWND getWindow(QString name);
QRect windowRect(HWND window);
QString windowTitle(HWND window);
QString windowText(HWND window);

#endif // WINDOWS_UTILS_H
