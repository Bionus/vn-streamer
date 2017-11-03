#include "streamer-server-client.h"
#include <QBuffer>
#include "logger.h"


StreamerServerClient::StreamerServerClient(QWebSocket *socket)
	: m_socket(socket)
{
	m_socket->setParent(this);

	connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
	connect(m_socket, SIGNAL(textMessageReceived(QString)), this, SLOT(textMessageReceived(QString)));
}

QString StreamerServerClient::toString() const
{
	return QString("'%1' (%2:%3)")
			.arg(m_socket->peerName())
			.arg(m_socket->peerAddress().toString())
			.arg(m_socket->peerPort());
}

void StreamerServerClient::sendText(const QString &text, const QString &name)
{
	int sent = m_socket->sendTextMessage("text|" + name + "|" + text);
	if (sent <= 0)
		LOG("Error sending text data", Logger::Error);
}

void StreamerServerClient::sendImage(const QPixmap &image)
{
	QByteArray img;
	QBuffer buffer(&img);
	buffer.open(QIODevice::WriteOnly);

	if (!m_size.isEmpty())
		image.scaled(m_size, Qt::KeepAspectRatio).save(&buffer, "JPG");
	else
		image.save(&buffer, "JPG");

	int sent = m_socket->sendTextMessage("image");
	if (sent <= 0)
	{
		LOG("Error sending image header", Logger::Error);
		return;
	}

	sent = m_socket->sendBinaryMessage(img);
	if (sent < img.length())
		LOG("Error sending image data", Logger::Error);
}

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
		return;
	}

	LOG(QString("Message received: type %1, args '%2'").arg(QString::number(action)).arg(args.join("', '")), Logger::Debug);
	emit commandReceived(action, args);
}
