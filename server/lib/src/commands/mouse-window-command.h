#ifndef MOUSE_WINDOW_COMMAND_H
#define MOUSE_WINDOW_COMMAND_H

#include <QPoint>
#include "window-command.h"


class MouseWindowCommand : public WindowCommand
{
	public:
		explicit MouseWindowCommand(Qt::MouseButton button, QPoint pos);
		virtual void execute(WindowController *controller) override;

	private:
		Qt::MouseButton m_button;
		QPoint m_pos;
};

#endif // MOUSE_WINDOW_COMMAND_H
