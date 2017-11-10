#ifndef COMPOSITE_WINDOW_COMMAND_H
#define COMPOSITE_WINDOW_COMMAND_H

#include <QList>
#include "window-command.h"


class CompositeWindowCommand : public WindowCommand
{
	public:
		explicit CompositeWindowCommand(QList<WindowCommand*> commands);
		virtual void execute(WindowController *controller) override;

	private:
		QList<WindowCommand*> m_commands;
};

#endif // COMPOSITE_WINDOW_COMMAND_H
