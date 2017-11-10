#ifndef PROFILE_H
#define PROFILE_H

#include <QSettings>
#include "commands/window-command.h"


class Profile
{
	public:
		explicit Profile(QSettings *settings);
		~Profile();
		QString name() const;
		QString path() const;
		WindowCommand *command(const QString &key) const;

	private:
		QSettings *m_settings;
		QMap<QString, WindowCommand*> m_commands;
};

#endif // PROFILE_H
