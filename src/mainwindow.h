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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

#include "appsettings.h"
#include "vk/vkservice.h"
#include "synchservice.h"
#include "ui_mainwindow.h"
#include "audiolistmodel.h"
#include "audioitemdelegate.h"
#include "profilemodel.h"
#include "about.h"

//use in impl.
#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <QLocale>
#include <QStatusBar>

namespace Ui
{
    class MainWindow;
}

namespace VkAudioSync
{
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    About *m_pAbout;

    VK::VKService *m_pVkService;
    AudioListModel *m_pAudioModel;
    AudioItemDelegate *m_pAudioItemDelegate;
    AppSettings *m_pAppSettings;
    SynchService *m_pSynchService;
    ProfileModel *m_pProfileModel;
    QDir *m_pDir;
    QTranslator *m_pTranslator;
    QStatusBar *m_pStatusBar;
    bool m_logined;

    void runSynch();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void restore();
    void retranslateUi();

 public slots:
    void slotLoginLogaut();
    void slotChooseDir();
    void slotStartSynch();
    void slotSettings();
    void slotAbout();
    void slotExit();
    void slotLoginSuccess(const QByteArray &xml);
    void slotAudioListLoaded(const QByteArray &xml);
    void slotProfileLoaded(const QByteArray &xml);
    void slotLoginUnsuccess();
    void slotPhotoLoaded();
    void slotPhotoMediumLoaded();
    void slotLanguageChanged(QString text);
    void slotAuthWindowOpened();
    void slotAuthWindowClosed();
    void slotRememberCheckboxChanged(bool checked);
    void slotUpdateStatusBar(const QString status);
    void slotShowAbs();
    void slotSortChanged(int option);
};
}

#endif // MAINWINDOW_H
