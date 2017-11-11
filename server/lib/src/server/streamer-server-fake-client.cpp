#include "streamer-server-fake-client.h"


StreamerServerFakeClient::StreamerServerFakeClient(const QString &name)
	: m_name(name)
{}

QString StreamerServerFakeClient::toString() const
{
	return m_name;
}

void StreamerServerFakeClient::sendCommand(const QString &message)
{
	textMessageReceived(message);
}

void StreamerServerFakeClient::sendText(const QString &text, const QString &name)
{
	// No-op
}

void StreamerServerFakeClient::sendImage(const QPixmap &image)
{
	// No-op
}
