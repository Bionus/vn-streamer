#ifndef WINDOW_COMMAND_H
#define WINDOW_COMMAND_H

#include "window-controller.h"


class WindowCommand
{
	public:
		virtual void execute(WindowController *controller) = 0;
};

#endif // WINDOW_COMMAND_H
