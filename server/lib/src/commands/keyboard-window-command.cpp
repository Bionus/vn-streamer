#include "keyboard-window-command.h"


KeyboardWindowCommand::KeyboardWindowCommand(Qt::Key key)
	: m_key(key)
{}

void KeyboardWindowCommand::execute(WindowController *controller)
{
	controller->keyPress(m_key);
}
