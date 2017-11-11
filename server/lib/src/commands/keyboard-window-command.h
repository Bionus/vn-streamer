#ifndef KEYBOARD_WINDOW_COMMAND_H
#define KEYBOARD_WINDOW_COMMAND_H

#include "window-command.h"


class KeyboardWindowCommand : public WindowCommand
{
	public:
		explicit KeyboardWindowCommand(Qt::Key key);
		virtual void execute(WindowController *controller) override;

	private:
		Qt::Key m_key;
};

#endif // KEYBOARD_WINDOW_COMMAND_H
