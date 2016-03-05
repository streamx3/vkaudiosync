/*
  *  VKAudioSync
  *  Copyright (C) 2011 Denis Medved <thebucefal@gmail.com>
  *
  *  This program is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"

namespace VkAudioSync
{
MainWindow::MainWindow(QWidget *parent /*=0*/) :
    QMainWindow(parent)
{
    m_pTranslator = new QTranslator(this);
    QApplication::instance()->installTranslator(m_pTranslator);

    m_pAbout = new About(this);
    m_pAppSettings = new AppSettings(this);
    m_pVkService = new VK::VKService(this);
    m_pAudioModel = new AudioListModel(this);
    m_pAudioItemDelegate = new AudioItemDelegate(this);
    m_pSynchService = new SynchService(this);
    m_pProfileModel = new ProfileModel;
    m_pStatusBar = new QStatusBar(this);

    setStatusBar(m_pStatusBar);
    m_pSynchService->setAudioModel(m_pAudioModel);

    ui = new Ui::MainWindow;
    ui->setupUi(this);

    ui->syncButton->setDisabled(true);

    // connects
    connect(ui->loginButton, SIGNAL(clicked()),
                this,SLOT(slotLoginLogaut())
    );
    connect(ui->selectButton, SIGNAL(clicked()),
            this,SLOT(slotChooseDir())
    );
    connect(ui->syncButton, SIGNAL(clicked()),
            this,SLOT(slotStartSynch())
    );
    connect(ui->aboutButton,SIGNAL(clicked()),
            this,SLOT(slotAbout())
    );
    connect(ui->exitButton,SIGNAL(clicked()),
            this,SLOT(slotExit())
    );
    connect(ui->langList,SIGNAL(currentIndexChanged(QString)),
            this, SLOT(slotLanguageChanged(QString))
    );
    connect(ui->remember,SIGNAL(clicked(bool)),
            this, SLOT(slotRememberCheckboxChanged(bool))
    );
    connect(ui->sorter, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotSortChanged(int))
    );
    connect(m_pSynchService, SIGNAL(updateStatusBar(const QString)),
            this, SLOT(slotUpdateStatusBar(const QString))
    );

    //VK SERVICE
    connect(m_pVkService,SIGNAL(loginSuccess(const QByteArray)),
            this,SLOT(slotLoginSuccess(QByteArray))
    );
    connect(m_pVkService,SIGNAL(audioListLoaded(const QByteArray)),
            this,SLOT(slotAudioListLoaded(QByteArray))
    );
    connect(m_pVkService,SIGNAL(profileLoaded(const QByteArray)),
            this,SLOT(slotProfileLoaded(QByteArray))
    );
    connect(m_pVkService,SIGNAL(opened()),
            this,SLOT(slotAuthWindowOpened())
    );
    connect(m_pVkService,SIGNAL(closed()),
            this,SLOT(slotAuthWindowClosed())
    );
    connect(m_pVkService,SIGNAL(loginUnsuccess()),
            this,SLOT(slotLoginUnsuccess())
    );
    //VK PROFILE
    connect(m_pProfileModel, SIGNAL(photoMediumLoaded()),
            this, SLOT(slotPhotoMediumLoaded())
    );

    ui->listView->setItemDelegate(m_pAudioItemDelegate);
    ui->listView->setModel(m_pAudioModel);
    ui->listView->setSelectionBehavior(QListView::SelectRows);

    int desktopWidth = QApplication::desktop()->width();
    int desktoHeight = QApplication::desktop()->height();
    move((desktopWidth-width()) / 2 , (desktoHeight - height()) / 3);

    m_pAppSettings->load();
    m_pVkService->setCookieJar(m_pAppSettings->cookieJar());

    m_logined = false;

    m_pDir = new QDir;
    m_pSynchService->setDir(m_pDir);

    restore();


    m_pSynchService->slotModelItemChanged();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_pDir;
    delete m_pAppSettings;
    delete m_pVkService;
    delete m_pAudioModel;
    delete m_pAudioItemDelegate;
    delete m_pSynchService;
    delete m_pProfileModel;
    delete m_pTranslator;
    delete m_pStatusBar;
}

void MainWindow::restore()
{
    QString language = m_pAppSettings->value("/general/language",QVariant("English")).toString();
    int languageIndex = ui->langList->findText(language);
    if (languageIndex != -1)
        ui->langList->setCurrentIndex(languageIndex);

    bool remember = m_pAppSettings->value("/general/remember",QVariant(false)).toBool();
    ui->remember->setChecked(remember);
    if (remember) {
        QString dir = m_pAppSettings->value("/general/dir").toString();
        m_pAppSettings->restore();
        m_pVkService->login(false);
        if (!dir.isEmpty() && QFile::exists(dir)) {
            m_pDir->setPath(dir);
            ui->syncButton->setDisabled(false);
        }
    } else
        m_pAppSettings->clearCookies();

    int sort = m_pAppSettings->value("sort", QVariant(0)).toInt();
    m_pAudioModel->setSortBy(sort);
}

void MainWindow::runSynch()
{
    m_pSynchService->synchronize();
}

void MainWindow::slotLoginLogaut()
{
    if (m_logined) {
        m_pAudioModel->clear();
        m_pVkService->logout();

        ui->selectButton->setDisabled(true);
        ui->syncButton->setDisabled(true);
        ui->loginButton->setText(tr("Login"));
        ui->username->clear();
        QPixmap map(ui->userpic->width(), ui->userpic->height());
        QImage defaultImg(":/application/share/icons/hicolor/64x64/apps/vkaudiosync.png");
        map.convertFromImage(defaultImg);
        ui->userpic->setPixmap(map);

        m_pAppSettings->cookieJar()->clear();

        m_logined = false;
    } else {
        m_pVkService->login();
    }
}

void MainWindow::slotChooseDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"),"", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        m_pDir->setPath(dir);
        m_pSynchService->setDir(m_pDir);
        ui->syncButton->setDisabled(false);
        m_pSynchService->setStatuses();
        m_pAppSettings->setValue("/general/dir",dir);
    }
}

void MainWindow::slotStartSynch()
{
    ui->syncButton->setDisabled(true);
    runSynch();
}

void MainWindow::slotSettings()
{
    QMessageBox::information(this,"Not work","temporary not work");
}

void MainWindow::slotAbout()
{
    m_pAbout->show();
}

void MainWindow::slotExit()
{
    ui->exitButton->setDisabled(true);
    m_pSynchService->stopSync();
    QApplication::instance()->exit();
}

void MainWindow::slotLoginSuccess(const QByteArray &/*xml*/)
{
    ui->loginButton->setText(tr("Logout"));
    ui->selectButton->setDisabled(false);
    m_logined = true;
}

void MainWindow::slotAudioListLoaded(const QByteArray &xml)
{
    m_pAudioModel->parseXml(xml);
    m_pSynchService->setStatuses();
}

void MainWindow::slotProfileLoaded(const QByteArray &xml)
{
    m_pProfileModel->parseXml(xml);
    if (!m_pProfileModel->firsrtName().isEmpty()) {
        QString name = QString("%1 %2")
                        .arg(m_pProfileModel->firsrtName())
                        .arg(m_pProfileModel->lastName());
        ui->username->setText(name);
    }
}

void MainWindow::slotLoginUnsuccess()
{
    ui->loginButton->setText(tr("Login"));
    m_logined = false;
}

void MainWindow::slotPhotoLoaded()
{
    //
}

void MainWindow::slotPhotoMediumLoaded()
{
    QPixmap pixmap(ui->userpic->width(),ui->userpic->height());

    if (pixmap.convertFromImage(m_pProfileModel->photoMedium()) ) {
        ui->userpic->setPixmap(pixmap);
    }
}

void MainWindow::slotLanguageChanged(QString text)
{
    Q_UNUSED(text);

    enum LANG_ID{
        LANG_ID_ENGLISH = 0,
        LANG_ID_RUSSIAN,
        LANG_ID_UKRAINIAN,
        LANG_ID_POLISH,
        LANG_ID_BELORUSIAN
    };

    m_pAppSettings->setValue("/general/language",ui->langList->currentText());
    QString translationPath = m_pAppSettings->translationPath();
    if (QFile::exists(translationPath)) {
        int langId = ui->langList->currentIndex();
        QString filename;

        switch(langId) {
        case LANG_ID_POLISH:
            filename = "main_pl.qm";
            break;
        case LANG_ID_BELORUSIAN:
            filename = "main_be.qm";
            break;
        case LANG_ID_UKRAINIAN:
            filename = "main_uk.qm";
            break;
        case LANG_ID_RUSSIAN:
            filename = "main_ru.qm";
            break;
        default:
            filename = "main_en.qm";
        }

        QString filePath = translationPath + QDir::separator() + filename;

        if (QFile::exists(filePath)) {
            m_pTranslator->load(filename,translationPath);
            retranslateUi();
        }
    }
}

void MainWindow::retranslateUi()
{
    ui->retranslateUi(this);
    m_pAbout->retranslateUi();
    m_pSynchService->slotModelItemChanged();
}

void MainWindow::slotAuthWindowOpened()
{
    ui->loginButton->setDisabled(true);
}

void MainWindow::slotAuthWindowClosed()
{
    ui->loginButton->setDisabled(false);
}

void MainWindow::slotRememberCheckboxChanged(bool checked)
{
    m_pAppSettings->setValue("/general/remember",QVariant(checked));
}

void MainWindow::slotUpdateStatusBar(const QString status)
{
    statusBar()->showMessage(status);
}

void MainWindow::slotShowAbs()
{
    //-
}

void MainWindow::slotSortChanged(int option)
{
    m_pAppSettings->setValue("sort", QVariant(option));
    m_pAudioModel->setSortBy(option);
}
}
