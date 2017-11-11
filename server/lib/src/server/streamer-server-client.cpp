#include "streamer-server-client.h"
#include "logger.h"


void StreamerServerClient::textMessageReceived(QString message)
{
	QStringList args = message.split(' ');
	QString command = args.takeFirst();
	Command action = (Command)command.toInt();

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
