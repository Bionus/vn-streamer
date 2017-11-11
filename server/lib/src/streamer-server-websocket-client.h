#ifndef STREAMER_SERVER_WEBSOCKET_CLIENT_H
#define STREAMER_SERVER_WEBSOCKET_CLIENT_H

#include <QtWebSockets/QWebSocket>
#include "streamer-server-client.h"


class StreamerServerWebsocketClient : public StreamerServerClient
{
	Q_OBJECT

	public:
		explicit StreamerServerWebsocketClient(QWebSocket *socket);
		virtual QString toString() const override;

	public slots:
		virtual void sendText(const QString &text, const QString &name = QString()) override;
		virtual void sendImage(const QPixmap &image) override;

	private:
		QWebSocket *m_socket;
};

#endif // STREAMER_SERVER_WEBSOCKET_CLIENT_H
