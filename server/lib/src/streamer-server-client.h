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
		virtual QString toString() const = 0;

	public slots:
		virtual void sendText(const QString &text, const QString &name = QString()) = 0;
		virtual void sendImage(const QPixmap &image) = 0;

	protected slots:
		void textMessageReceived(QString message);

	signals:
		void disconnected();
		void commandReceived(Command command, QStringList args);

	protected:
		QSize m_size;
};

#endif // STREAMER_SERVER_CLIENT_H
