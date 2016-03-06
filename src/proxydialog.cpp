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

#include "proxydialog.h"

VkAudioSync::ProxyDialog::ProxyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProxyDialog)
{
    ui->setupUi(this);

    m_proxy.setType(QNetworkProxy::HttpProxy);

    QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);
}

VkAudioSync::ProxyDialog::~ProxyDialog()
{
    delete ui;
}

void VkAudioSync::ProxyDialog::retranslateUi(){
    ui->retranslateUi(this);
}

void VkAudioSync::ProxyDialog::setSettings(AppSettings *AppSettingsPtr){
    if(AppSettingsPtr != NULL){
        this->m_pSettings = AppSettingsPtr;
        this->applySettings();
    }
}

void VkAudioSync::ProxyDialog::on_buttonBox_accepted()
{
    m_pSettings->setValue("prx_use_proxy", ui->checkBoxUseProxy->isChecked());
    m_pSettings->setValue("prx_use_authorisation", ui->checkBoxAuthorisation->isChecked());
    m_pSettings->setValue("prx_host", ui->lineEditHost->text());
    m_pSettings->setValue("prx_port", ui->lineEditPort->text());
    m_pSettings->setValue("prx_username", ui->lineEditUsername->text());
    m_pSettings->setValue("prx_password", ui->lineEditPassword->text());
    this->applySettings();
    close();
}

void VkAudioSync::ProxyDialog::applySettings()
{
    if(m_pSettings->value("prx_use_proxy").toBool()){
        m_proxy.setHostName(m_pSettings->value("prx_host").toString());
        m_proxy.setPort(m_pSettings->value("prx_port").toString().toInt());
        if(m_pSettings->value("prx_use_authorisation").toBool()){
            m_proxy.setUser(m_pSettings->value("prx_username").toString());
            m_proxy.setPassword(m_pSettings->value("prx_password").toString());
        }else{
            m_proxy.setUser("");
            m_proxy.setPassword("");
        }
        QNetworkProxy::setApplicationProxy(m_proxy);
    }else{
        QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);
    }
}

void VkAudioSync::ProxyDialog::on_checkBoxUseProxy_stateChanged(int arg1)
{
    this->updateVisibility(arg1 ? true : false, ui->checkBoxAuthorisation->isChecked());
}

void VkAudioSync::ProxyDialog::on_buttonBox_rejected()
{
    this->close();
}

void VkAudioSync::ProxyDialog::updateVisibility(bool use_proxy, bool use_auth)
{
    ui->lineEditHost->setEnabled(use_proxy);
    ui->lineEditPort->setEnabled(use_proxy);
    ui->checkBoxAuthorisation->setEnabled(use_proxy);

    if(use_proxy){
        ui->lineEditUsername->setEnabled(use_auth);
        ui->lineEditPassword->setEnabled(use_auth);
    }else{
        ui->lineEditPassword->setEnabled(false);
        ui->lineEditUsername->setEnabled(false);
    }

}

void VkAudioSync::ProxyDialog::on_checkBoxAuthorisation_stateChanged(int arg1)
{
    this->updateVisibility(ui->checkBoxUseProxy->isChecked(),arg1 ? true : false);

}
