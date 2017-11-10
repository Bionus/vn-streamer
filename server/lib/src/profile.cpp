#include "profile.h"
#include "commands/window-command-factory.h"


Profile::Profile(QSettings *settings)
	: m_settings(settings)
{
	settings->beginGroup("Commands");
	QStringList keys = settings->childKeys();
	for (const QString &key : keys)
	{
		QString value = m_settings->value(key).toString();
		WindowCommand *command = WindowCommandFactory::create(value);
		m_commands.insert(key, command);
	}
	settings->endGroup();
}

Profile::~Profile()
{
	qDeleteAll(m_commands);
}

QString Profile::name() const
{
	return m_settings->value("name").toString();
}

QString Profile::path() const
{
	return m_settings->value("path").toString();
}

WindowCommand *Profile::command(const QString &key) const
{
	return m_commands.value(key, Q_NULLPTR);
}
