#include "streamer-server-client.h"
#include "logger.h"


void StreamerServerClient::textMessageReceived(QString message)
{
	QStringList args = message.split(' ');
	QString command = args.takeFirst();

	bool ok;
	Command action = (Command)command.toInt(&ok);
	if (!ok)
	{
		static QMap<QString, Command> actions = {
			{ "setup", Command::Setup },
			{ "next", Command::Next },
			{ "close", Command::Close },
			{ "close_all", Command::CloseAll },
		};

		if (!actions.contains(command))
		{
			LOG(QString("Invalid command action '%1'").arg(command), Logger::Warning);
			return;
		}
		action = actions[command];
	}

	if (action < 0 || action > Command::CloseAll)
	{
		LOG(QString("Unknown message received: '%1'").arg(message), Logger::Warning);
		return;
	}

	if (action == Command::Setup)
	{
		if (args.count() != 2)
		{
			LOG("Invalid setup command received", Logger::Warning);
			return;
		}
		m_size = QSize(args[0].toInt(), args[1].toInt());
	}

	LOG(QString("Message received: type %1, args '%2'").arg(QString::number(action)).arg(args.join("', '")), Logger::Debug);
	emit commandReceived(action, args);
}
