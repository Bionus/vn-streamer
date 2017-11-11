#include "visual-novel-controller.h"
#include "logger.h"


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

void VisualNovelController::executeCommand(QString name)
{
	WindowCommand *command = m_profile->command(name);
	if (command == Q_NULLPTR)
	{
		LOG(QString("Undefined command '%1'").arg(name), Logger::Error);
		return;
	}

	command->execute(m_window);
}

void VisualNovelController::next()
{
	if (m_profile != Q_NULLPTR)
		executeCommand("next");
	else
		m_window->click(Qt::LeftButton, QPoint(100, 100));
}

void VisualNovelController::hideTextbox()
{
	if (m_profile != Q_NULLPTR)
		executeCommand("hide_textbox");
	else
		m_window->click(Qt::RightButton, QPoint(100, 100));
}

void VisualNovelController::showTextbox()
{
	if (m_profile != Q_NULLPTR)
		executeCommand("show_textbox");
	else
		m_window->click(Qt::RightButton, QPoint(100, 100));
}
