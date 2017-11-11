#include "streamer-server.h"
#include <QBuffer>
#include "logger.h"
#include "streamer-server-websocket-client.h"


StreamerServer::StreamerServer(int port, QObject *parent)
	: QObject(parent), m_port(port)
{
	m_server = new QWebSocketServer("Streamer Server", QWebSocketServer::NonSecureMode, this);
	connect(m_server, &QWebSocketServer::newConnection, this, &StreamerServer::newConnection);
}

void StreamerServer::addClient(StreamerServerClient *client)
{
	connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
	connect(client, SIGNAL(commandReceived(Command, QStringList)), this, SLOT(clientCommandReceived(Command, QStringList)));

	m_clients.append(client);
}

void StreamerServer::start()
{
	LOG("Starting server", Logger::Info);
	m_server->listen(QHostAddress::Any, m_port);
	LOG(QString("Server started on port %1").arg(m_port), Logger::Info);
}

void StreamerServer::stop()
{
	LOG("Stopping server", Logger::Info);
	m_server->close();
	LOG("Server stopped", Logger::Info);
}

void StreamerServer::sendText(const QString &text, const QString &name)
{
	for (StreamerServerClient *client : m_clients)
		client->sendText(text, name);
}

void StreamerServer::sendImage(const QPixmap &image)
{
	for (StreamerServerClient *client : m_clients)
		client->sendImage(image);
}

void StreamerServer::newConnection()
{
	QWebSocket *socket = m_server->nextPendingConnection();
	auto client = new StreamerServerWebsocketClient(socket);

	addClient(client);

	LOG(QString("New client %1").arg(client->toString()), Logger::Info);
}

void StreamerServer::clientDisconnected()
{
	StreamerServerClient *client = qobject_cast<StreamerServerClient*>(sender());
	if (!client)
		return;

	LOG(QString("Client %1 disconnected").arg(client->toString()), Logger::Info);

	client->deleteLater();
	m_clients.removeAll(client);
}

void StreamerServer::clientCommandReceived(Command command, QStringList args)
{
	StreamerServerClient *client = qobject_cast<StreamerServerClient*>(sender());
	if (!client)
		return;

	emit commandReceived(client->toString(), command, args);
}
