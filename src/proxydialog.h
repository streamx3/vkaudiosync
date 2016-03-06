/*
  *  VKAudioSync
  *  Copyright (C) 2016 Andrew Shelestov <streamx3@gmail.com>
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


#ifndef PROXYDIALOG_H
#define PROXYDIALOG_H

#include <QDialog>
#include <QNetworkProxy>
#include "ui_proxydialog.h"
#include "appsettings.h"


namespace Ui {
class ProxyDialog;
}

namespace VkAudioSync
{
class ProxyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProxyDialog(QWidget *parent = 0);
    ~ProxyDialog();

    void retranslateUi();
    void setSettings(AppSettings *AppSettingsPtr);

protected:
    void applySettings();
    void updateVisibility(bool use_proxy, bool use_auth);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_checkBoxUseProxy_stateChanged(int arg1);
    void on_checkBoxAuthorisation_stateChanged(int arg1);

private:
    Ui::ProxyDialog *ui;

    QNetworkProxy m_proxy;

    AppSettings *m_pSettings;
};
}

#endif // PROXYDIALOG_H
