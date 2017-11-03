#ifndef LOGGER_H
#define LOGGER_H

#define LOG(a, b) Logger::getInstance().log((a), (b))


#include <QObject>
#include <QString>
#include <QFile>


class Logger : public QObject
{
	Q_OBJECT

	public:
		enum LogLevel
		{
			Debug = 0,
			Info = 1,
			Warning = 2,
			Error = 3
		};

		// Singleton pattern
		static Logger& getInstance()
		{
			static Logger instance;
			return instance;
		}
		Logger(Logger const&) = delete;
		void operator=(Logger const&) = delete;

		void setLogLevel(LogLevel level);
		void log(QString message, LogLevel type = Info);

	signals:
		void newLog(QString message);

	private:
		Logger();
		QFile m_logFile;
		LogLevel m_level = LogLevel::Info;
};

#endif // LOGGER_H
