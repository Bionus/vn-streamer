#include "translation-aggregator-controller.h"
#include "windows-utils.h"
#include "logger.h"
#include <QDebug>
#include "Commctrl.h"


TranslationAggregatorController::TranslationAggregatorController(HWND window, QObject *parent)
	: QObject(parent), m_window(window)
{
	HWND child = NULL, group;
	while ((group = FindWindowEx(m_window, child, 0, 0)) != NULL)
	{
		HWND translation = FindWindowEx(group, NULL, 0, 0);
		m_translations.append(translation);
		child = group;
	}

	if (m_translations.isEmpty())
		LOG("Could not find any of Translation Aggregator's translation windows", Logger::Error);
}

int TranslationAggregatorController::getTranslationCount() const
{
	return m_translations.count();
}

QString TranslationAggregatorController::getTranslation(int index) const
{
	return windowText(m_translations[index]);
}
