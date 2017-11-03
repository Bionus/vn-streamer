#ifndef TRANSLATION_AGGREGATOR_CONTROLLER_H
#define TRANSLATION_AGGREGATOR_CONTROLLER_H

#include <QObject>
#include <QList>
#include "windows.h"


class TranslationAggregatorController : public QObject
{
	Q_OBJECT

	public:
		explicit TranslationAggregatorController(HWND window, QObject *parent = Q_NULLPTR);
		int getTranslationCount() const;
		QString getTranslation(int index) const;

	private:
		HWND m_window;
		QList<HWND> m_translations;
};

#endif // TRANSLATION_AGGREGATOR_CONTROLLER_H
