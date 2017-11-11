#ifndef STREAMER_SERVER_FAKE_CLIENT_H
#define STREAMER_SERVER_FAKE_CLIENT_H

#include "streamer-server-client.h"


class StreamerServerFakeClient : public StreamerServerClient
{
	Q_OBJECT

	public:
		explicit StreamerServerFakeClient(const QString &name);
		virtual QString toString() const override;
		void sendCommand(const QString &message);

	public slots:
		virtual void sendText(const QString &text, const QString &name = QString()) override;
		virtual void sendImage(const QPixmap &image) override;

	private:
		QString m_name;
};

#endif // STREAMER_SERVER_FAKE_CLIENT_H
