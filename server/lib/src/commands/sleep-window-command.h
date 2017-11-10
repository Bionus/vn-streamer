#ifndef SLEEP_WINDOW_COMMAND_H
#define SLEEP_WINDOW_COMMAND_H

#include "window-command.h"


class SleepWindowCommand : public WindowCommand
{
	public:
		explicit SleepWindowCommand(int duration);
		virtual void execute(WindowController *controller) override;

	private:
		int m_duration;
};

#endif // SLEEP_WINDOW_COMMAND_H
