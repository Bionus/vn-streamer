#include "streamer-server-websocket-client.h"
#include <QBuffer>
#include "logger.h"


StreamerServerWebsocketClient::StreamerServerWebsocketClient(QWebSocket *socket)
	: m_socket(socket)
{
	m_socket->setParent(this);

	connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
	connect(m_socket, SIGNAL(textMessageReceived(QString)), this, SLOT(textMessageReceived(QString)));
}

QString StreamerServerWebsocketClient::toString() const
{
	QString ip = QString("%1:%2")
				 .arg(m_socket->peerAddress().toString())
				 .arg(m_socket->peerPort());

	if (m_socket->peerName().isEmpty())
		return ip;

	return QString("'%1' (%2)").arg(m_socket->peerName()).arg(ip);
}

void StreamerServerWebsocketClient::sendText(const QString &text, const QString &name)
{
	int sent = m_socket->sendTextMessage("text|" + name + "|" + text);
	if (sent <= 0)
		LOG("Error sending text data", Logger::Error);
}

void StreamerServerWebsocketClient::sendImage(const QPixmap &image)
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
