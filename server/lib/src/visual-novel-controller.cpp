#include "visual-novel-controller.h"


VisualNovelController::VisualNovelController(WindowController *window, QSettings *settings, Profile *profile, QObject *parent)
	: QObject(parent), m_window(window), m_settings(settings), m_profile(profile)
{}

QPixmap VisualNovelController::getImage()
{
	bool hideBox = m_settings->value("hideTextbox", true).toBool();
	if (hideBox)
		hideTextbox();

	QPixmap pixmap = m_window->screenshot();

	if (hideBox)
		showTextbox();

	return pixmap;
}

void VisualNovelController::next()
{
	if (m_profile != Q_NULLPTR)
		m_profile->command("next")->execute(m_window);
	else
		m_window->click(Qt::LeftButton, QPoint(100, 100));
}

void VisualNovelController::hideTextbox()
{
	if (m_profile != Q_NULLPTR)
		m_profile->command("hide_textbox")->execute(m_window);
	else
		m_window->click(Qt::RightButton, QPoint(100, 100));
}

void VisualNovelController::showTextbox()
{
	if (m_profile != Q_NULLPTR)
		m_profile->command("show_textbox")->execute(m_window);
	else
		m_window->click(Qt::RightButton, QPoint(100, 100));
}
