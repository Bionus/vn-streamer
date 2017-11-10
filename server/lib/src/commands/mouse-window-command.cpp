#include "mouse-window-command.h"


MouseWindowCommand::MouseWindowCommand(Qt::MouseButton button, QPoint pos)
	: m_button(button), m_pos(pos)
{}

void MouseWindowCommand::execute(WindowController *controller)
{
	controller->click(m_button, m_pos);
}
