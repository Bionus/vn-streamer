#ifndef STREAMER_SERVER_H
#define STREAMER_SERVER_H

#include <QObject>
#include <QtWebSockets/QWebSocketServer>
#include <QPixmap>
#include "streamer-server-client.h"
#include "command.h"


class StreamerServer : public QObject
{
	Q_OBJECT

	public:
		explicit StreamerServer(int port, QObject *parent = Q_NULLPTR);
		void addClient(StreamerServerClient *client);

	public slots:
		void start();
		void stop();
		void sendText(const QString &text, const QString &name = QString());
		void sendImage(const QPixmap &image);

	protected slots:
		void newConnection();
		void clientDisconnected();
		void clientCommandReceived(Command command, QStringList args);

	signals:
		void commandReceived(QString client, Command command, QStringList args);

	private:
		int m_port;
		QWebSocketServer *m_server;
		QList<StreamerServerClient*> m_clients;
};

#endif // STREAMER_SERVER_H
