#ifndef WINDOW_COMMAND_FACTORY_H
#define WINDOW_COMMAND_FACTORY_H

#include <QString>
#include "window-command.h"


class WindowCommandFactory
{
	public:
		static WindowCommand *create(const QString &command);

	private:
		static WindowCommand *makeSingle(const QString &command);
};

#endif // WINDOW_COMMAND_FACTORY_H
