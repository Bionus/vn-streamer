#ifndef OPTIONS_WINDOW_H
#define OPTIONS_WINDOW_H

#include <QDialog>
#include <QSettings>


namespace Ui
{
	class OptionsWindow;
}

class OptionsWindow : public QDialog
{
	Q_OBJECT

	public:
		explicit OptionsWindow(QSettings *settings, QWidget *parent = Q_NULLPTR);
		~OptionsWindow();

	public slots:
		void accept();

	private:
		Ui::OptionsWindow *ui;
		QSettings *m_settings;
};

#endif // OPTIONS_WINDOW_H
