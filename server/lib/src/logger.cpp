#include "logger.h"
#include <QDateTime>

#ifdef QT_DEBUG
	#include <QDebug>
#endif


Logger::Logger()
{
	m_logFile.setFileName("main.log");
	m_logFile.open(QFile::Append | QFile::Text | QFile::Truncate);
}

void Logger::setLogLevel(LogLevel level)
{
	m_level = level;
}

void Logger::log(QString message, LogLevel level)
{
	if (level < m_level)
		return;

	static const QStringList levels = QStringList() << "Debug" << "Info" << "Warning" << "Error";
	static const QStringList colors = QStringList() << "#999" << "" << "orange" << "red";
	QString levelStr = levels[level];
	QDateTime time = QDateTime::currentDateTime();

	// Write ASCII log to file
	m_logFile.write(QString("[" + time.toString("hh:mm:ss.zzz") + "][" + levelStr + "] " + message + "\n").toUtf8());
	m_logFile.flush();

	// Emit colored HTML log
	QString levelColor = colors[level];
	QString msg = "[" + time.toString("hh:mm:ss.zzz") + "][" + levelStr + "] " + message;
	if (!levelColor.isEmpty())
		msg = QString("<span style='color:%1'>%2</span>").arg(levelColor).arg(msg);
	emit newLog(msg);

	#ifdef QT_DEBUG
		qDebug() << time.toString("hh:mm:ss.zzz") << levelStr << message;
	#endif
}
