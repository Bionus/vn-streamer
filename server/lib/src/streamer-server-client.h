#ifndef STREAMER_SERVER_CLIENT_H
#define STREAMER_SERVER_CLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QPixmap>
#include "command.h"


class StreamerServerClient : public QObject
{
	Q_OBJECT

	public:
		explicit StreamerServerClient(QWebSocket *socket);
		QString toString() const;

	public slots:
		void sendText(const QString &text, const QString &name = QString());
		void sendImage(const QPixmap &image);

	protected slots:
		void textMessageReceived(QString message);

	signals:
		void disconnected();
		void commandReceived(Command command, QStringList args);

	private:
		QWebSocket *m_socket;
		QSize m_size;
};

#endif // STREAMER_SERVER_CLIENT_H
