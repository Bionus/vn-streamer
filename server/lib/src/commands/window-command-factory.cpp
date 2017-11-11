#include "window-command-factory.h"
#include <QMap>
#include "logger.h"
#include "composite-window-command.h"
#include "sleep-window-command.h"
#include "mouse-window-command.h"
#include "keyboard-window-command.h"


WindowCommand *WindowCommandFactory::create(const QStringList &values)
{
	QList<WindowCommand*> list;
	for (const QString &value : values)
	{
		WindowCommand *command = makeSingle(value);
		if (command != Q_NULLPTR)
			list.append(command);
	}

	if (list.isEmpty())
		return Q_NULLPTR;
	if (list.count() > 1)
		return new CompositeWindowCommand(list);
	return list.first();
}

WindowCommand *WindowCommandFactory::makeSingle(const QString &command)
{
	if (command.isEmpty())
		return Q_NULLPTR;

	QStringList data = command.split(' ', QString::SkipEmptyParts);
	QString type = data.takeFirst();

	// Sleep commands
	if (type == "sleep" && data.count() == 1)
	{
		int duration = data[0].toInt();
		return new SleepWindowCommand(duration);
	}

	// Keyboard commands
	if (type == "key")
	{
		static QMap<QString, Qt::Key> keys = {
			{ "enter", Qt::Key_Enter },
			{ "escape", Qt::Key_Escape },
			{ "space", Qt::Key_Space },
		};

		if (!keys.contains(data[0]))
		{
			LOG(QString("Invalid key '%1'").arg(data[0]), Logger::Error);
			return Q_NULLPTR;
		}

		Qt::Key key = keys[data[0]];
		return new KeyboardWindowCommand(key);
	}

	// Mouse commands
	if (type == "mouse" && data.count() == 3)
	{
		static QMap<QString, Qt::MouseButton> buttons = {
			{ "left", Qt::LeftButton },
			{ "right", Qt::RightButton },
		};

		if (!buttons.contains(data[0]))
		{
			LOG(QString("Invalid button '%1'").arg(data[0]), Logger::Error);
			return Q_NULLPTR;
		}

		Qt::MouseButton button = buttons[data[0]];
		QPoint pos(data[1].toInt(), data[2].toInt());
		return new MouseWindowCommand(button, pos);
	}

	LOG(QString("Invalid command '%1'").arg(command), Logger::Error);
	return Q_NULLPTR;
}
