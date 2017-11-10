#include "composite-window-command.h"


CompositeWindowCommand::CompositeWindowCommand(QList<WindowCommand *> commands)
	: m_commands(commands)
{}

void CompositeWindowCommand::execute(WindowController *controller)
{
	for (WindowCommand *command : m_commands)
		command->execute(controller);
}
