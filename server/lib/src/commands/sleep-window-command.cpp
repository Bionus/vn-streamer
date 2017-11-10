#include "sleep-window-command.h"
#include <QThread>


SleepWindowCommand::SleepWindowCommand(int duration)
	: m_duration(duration)
{}

void SleepWindowCommand::execute(WindowController *controller)
{
	Q_UNUSED(controller);

	QThread::msleep(m_duration);
}
