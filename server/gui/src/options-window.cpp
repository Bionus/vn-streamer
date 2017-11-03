#include "options-window.h"
#include "ui_options-window.h"


OptionsWindow::OptionsWindow(QSettings *settings, QWidget *parent)
	: QDialog(parent), ui(new Ui::OptionsWindow), m_settings(settings)
{
	ui->setupUi(this);

	ui->spinPort->setValue(m_settings->value("port", 46421).toInt());
	ui->comboAspectRatio->setCurrentIndex(m_settings->value("aspectRatio", Qt::KeepAspectRatio).toInt());
	ui->checkScreenshots->setChecked(m_settings->value("screenshots", true).toBool());
	ui->checkHideTextbox->setChecked(m_settings->value("hideTextbox", true).toBool());
}

OptionsWindow::~OptionsWindow()
{
	delete ui;
}

void OptionsWindow::accept()
{
	m_settings->setValue("port", ui->spinPort->value());
	m_settings->setValue("aspectRatio", ui->comboAspectRatio->currentIndex());
	m_settings->setValue("screenshots", ui->checkScreenshots->isChecked());
	m_settings->setValue("hideTextbox", ui->checkHideTextbox->isChecked());

	m_settings->sync();
	QDialog::accept();
}
