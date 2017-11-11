#include "main-window.h"
#include "ui_main-window.h"
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include <QClipboard>
#include <QList>
#include <QDir>
#include <QFileIconProvider>
#include "logger.h"
#include "windows-utils.h"
#include "options-window.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(&Logger::getInstance(), &Logger::newLog, this, &MainWindow::newLog);

	LOG("Loading settings", Logger::Info);
	m_settings = new QSettings("settings.ini", QSettings::IniFormat, this);
	restoreGeometry(m_settings->value("Geometry/MainWindow").toByteArray());
	restoreState(m_settings->value("State/MainWindow").toByteArray());

	// Splitter sizes
	QList<int> sizes;
	QStringList splitSizes = m_settings->value("Splitter/LogVN", "300,200,100").toString().split(',');
	for (const QString &size : splitSizes)
		sizes.append(size.toInt());
	ui->splitterLogVN->setSizes(sizes);

	LOG("Loading profiles", Logger::Info);
	QDir profilesDir("profiles/");
	QStringList profiles = profilesDir.entryList(QStringList() << "*.ini", QDir::Files);
	for (const QString &file : profiles)
	{
		QSettings *profileSettings = new QSettings(profilesDir.absoluteFilePath(file), QSettings::IniFormat, this);
		profileSettings->setIniCodec("UTF-8");
		Profile *profile = new Profile(profileSettings);
		m_profiles.append(profile);
	}

	m_server = new StreamerServer(m_settings->value("port", 46421).toInt(), this);
	connect(m_server, &StreamerServer::commandReceived, this, &MainWindow::commandReceived);

	LOG("Looking for Translation Aggregator", Logger::Info);
	HWND taWindow = NULL;
	QMap<QString, HWND> windows = getAllWindows();
	for (const QString &win : windows.keys())
		if (win.startsWith("Translation Aggregator"))
			taWindow = windows[win];
	if (taWindow != NULL)
	{
		m_taController = new TranslationAggregatorController(taWindow, this);
		LOG(QString("%1 translation sources found in Translation Aggregator").arg(m_taController->getTranslationCount()), Logger::Info);
		for (int i = 0; i < m_taController->getTranslationCount(); ++i)
			ui->comboTextSource->addItem(QString("Translation Aggregator #%1").arg(i + 1));
	}
	else
		LOG("Cannot find Translation Aggregator", Logger::Warning);

	LOG("Starting poller", Logger::Info);
	m_windowPoller = new QTimer(this);
	connect(m_windowPoller, &QTimer::timeout, this, &MainWindow::pollWindows);
	m_windowPoller->start(3000);
	pollWindows();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QString splitterLogVN;
	QList<int> szes = ui->splitterLogVN->sizes();
	for (int size : szes)
		splitterLogVN += (!splitterLogVN.isEmpty() ? "," : "") + QString::number(size);

	LOG("Saving settings", Logger::Info);
	m_settings->setValue("State/MainWindow", saveState());
	m_settings->setValue("Geometry/MainWindow", saveGeometry());
	m_settings->setValue("Splitter/LogVN", splitterLogVN);
	m_settings->sync();

	QMainWindow::closeEvent(event);
}

#include <QThread>
void MainWindow::dataChanged()
{
	QThread::msleep(200);

	QString text;

	int textSource = ui->comboTextSource->currentIndex();
	if (textSource == 0)
		text = QApplication::clipboard()->text();
	else
		text = m_taController->getTranslation(textSource - 1);
	text = text.trimmed();

	static QStringList blacklisted = QStringList() << "Requesting Data.";
	if (text == m_lastText || blacklisted.contains(text))
	{
		if (text != m_lastText)
			QTimer::singleShot(200, this, SLOT(dataChanged()));
		return;
	}
	m_lastText = text;

	static QRegularExpression regex("[【\\[](.+?)[】\\]]\\s*[\"]?(.+?)[\"]?$");
	auto match = regex.match(text);
	if (match.hasMatch())
	{
		QString name = match.captured(1).trimmed();
		QString message = match.captured(2).trimmed();
		ui->textEditText->appendPlainText(QString("[%1] %2").arg(name).arg(message));
		m_server->sendText(message, name);
	}
	else
	{
		ui->textEditText->appendPlainText(text);
		m_server->sendText(text);
	}

	QPixmap img = m_vnController->getImage();
	m_server->sendImage(img);
	ui->labelImage->setPixmap(img.scaled(QSize(200, 150), Qt::KeepAspectRatio));
}

void MainWindow::newLog(QString message)
{
	ui->textEditLog->append(message + "<br/>");
}

void MainWindow::commandReceived(QString client, Command command, QStringList args)
{
	static QStringList commands = QStringList() << "Setup" << "Next" << "Close" << "CloseAll";

	QString cmd = commands[command];
	QString message = !args.isEmpty() ? QString("%2 (%3)").arg(cmd).arg(args.join(", ")) : cmd;
	ui->textEditCommands->append(QString("[%1] %2<br/>").arg(client).arg(message));

	if (command == Command::Next)
	{
		m_vnController->next();
	}
}

void MainWindow::pollWindows()
{
	m_windows = getAllWindows();

	QString current = ui->comboApplication->currentText();
	ui->comboApplication->clear();

	for (const QString &title : m_windows.keys())
	{
		HWND window = m_windows[title];
		if (!m_windowsProfiles.contains(window))
		{
			QString exe = getWindowExecutable(window);
			m_windowsExecutables.insert(window, exe);

			for (Profile *profile : m_profiles)
			{
				if (profile->match(exe))
				{
					m_windowsProfiles.insert(window, profile);
					break;
				}
			}
		}

		if (m_windowsProfiles.contains(window))
		{
			QString exe = m_windowsExecutables[window];
			QIcon icon = QFileIconProvider().icon(QFileInfo(exe));
			ui->comboApplication->addItem(icon, title);
		}
		else
		{
			ui->comboApplication->addItem(title);
		}
	}

	ui->comboApplication->setCurrentText(current);
}

void MainWindow::toggle()
{
	if (m_streaming == 0)
		start();
	else
		stop();
}
void MainWindow::start()
{
	// Start image poller
	HWND window = m_windows[ui->comboApplication->currentText()];
	Profile *profile = m_windowsProfiles.value(window, Q_NULLPTR);
	m_vnController = new VisualNovelController(new WindowController(window), m_settings, profile);
	ui->labelImage->setPixmap(m_vnController->getImage().scaled(QSize(200, 150), Qt::KeepAspectRatio));

	// Stop polling
	m_windowPoller->stop();
	ui->comboApplication->setEnabled(false);

	// Start server
	m_server->start();

	// Start listening to the clipboard
	connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &MainWindow::dataChanged);

	m_streaming = 1;
	ui->buttonStart->setText(tr("Stop"));
}
void MainWindow::stop()
{
	// Stop listening to the clipboard
	disconnect(QApplication::clipboard(), &QClipboard::dataChanged, this, &MainWindow::dataChanged);

	// Stop server
	m_server->stop();

	m_streaming = 0;
	ui->buttonStart->setText(tr("Start"));
	ui->buttonStart->setEnabled(true);

	// Start poller
	pollWindows();
	m_windowPoller->start();
	ui->comboApplication->setEnabled(true);
}

void MainWindow::fileSettings()
{
	OptionsWindow *ow = new OptionsWindow(m_settings, this);
	//connect(ow, &QDialog::accepted, this, &MainWindow::loadSettings);
	ow->show();
}
void MainWindow::aboutProgram()
{
}
void MainWindow::aboutGithub()
{
	QDesktopServices::openUrl(QUrl(PROJECT_GITHUB_URL));
}
void MainWindow::aboutReportBug()
{
	QDesktopServices::openUrl(QUrl(QString(PROJECT_GITHUB_URL) + "/issues/new"));
}
