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

#include "appsettings.h"

namespace VkAudioSync
{
AppSettings::AppSettings(QObject *parent /*=0*/) : QObject(parent)
{
    m_useConfig = false;

    m_pUserDir = new QDir;
    m_pAppDir = new QDir;
    m_pSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName(), this);
    m_pApp = QCoreApplication::instance();
    m_pCookieJar = new AppCookieJar(this);
    m_pCookieFile = new QFile(this);
    #ifdef Q_WS_X11
        m_pAppDir->setPath("/usr/share/vkaudiosync/");
        m_pUserDir->setPath(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));

        if (!m_pUserDir->exists(".vkaudiosync")) {
            if ( m_pUserDir->mkdir(".vkaudiosync")) {
                m_pUserDir->cd(".vkaudiosync");
            }
        } else {
            m_pUserDir->cd(".vkaudiosync");
        }
    #else
        m_pAppDir->setPath(m_pApp->applicationDirPath());
        m_pUserDir->setPath(m_pApp->applicationDirPath());
    #endif

}

AppSettings::~AppSettings()
{
    save();

    delete m_pCookieFile;
    delete m_pSettings;
    delete m_pAppDir;
    delete m_pUserDir;
    delete m_pCookieJar;
}

void AppSettings::load()
{
    QFile file;
    QString path;

    if (m_pUserDir->exists() && m_pUserDir->isReadable()) {
        path = m_pUserDir->absolutePath() + QDir::separator() + "settings.ini";
        file.setFileName(path);

        if (file.exists() && file.isWritable()) {
            m_pSettings->setPath(QSettings::IniFormat,QSettings::UserScope, path);
            m_useConfig = true;
        } else if (file.open(QIODevice::ReadWrite)) {
            file.close();
            m_pSettings->setPath(QSettings::IniFormat,QSettings::UserScope, path);
            m_pSettings->setUserIniPath(path);
            m_useConfig = true;
        } else {
            m_useConfig = false;
#ifndef Q_WS_WIN
            throw QString(tr("Permision denied: Can't write ") + path);
#endif
        }

        path = m_pUserDir->absolutePath() + QDir::separator() + "cookie.xml";
        file.setFileName(path);
        if (file.exists() && file.isWritable()) {
            setCookieFile(path);
        } else if (file.open(QIODevice::ReadWrite)) {
            file.close();
            setCookieFile(path);
        }
    } else {
#ifndef Q_WS_WIN
        throw QString(tr("Permision denied: Can't read ") + m_pUserDir->path());
#endif
    }

    restore();
}

void AppSettings::setValue(const QString & key, const QVariant & value )
{
    if (m_useConfig) {
        m_pSettings->setValue(key, value);
    }
}
QVariant AppSettings::value ( const QString & key, const QVariant & defaultValue/* = QVariant() */) const
{
    if (m_useConfig) {
        return m_pSettings->value(key, defaultValue);
    } else {
        return QVariant();
    }
}

AppCookieJar* AppSettings::cookieJar() const
{
    return m_pCookieJar;
}

void AppSettings::clearCookies()
{
    m_pCookieJar->clear();
}

void AppSettings::clear()
{
    // TODO Implement me!
}

void AppSettings::save()
{
    m_pCookieJar->save();
    if (m_useConfig) {
        m_pSettings->sync();
    }
}

void AppSettings::restore()
{
    m_pCookieJar->restore(); // TODO Consider removal of this function
}

void AppSettings::setCookieFile(QString path)
{
    m_pCookieFile->setFileName(path);
    m_pCookieJar->setFile(m_pCookieFile);
}

QDir* AppSettings::applicationPath()
{
    return m_pAppDir;
}

QDir* AppSettings::userPath()
{
    return m_pUserDir;
}

QString AppSettings::translationPath()
{
#ifdef Q_WS_X11
    if (QCoreApplication::instance()->applicationDirPath() != "/usr/bin")
        return "../share/vkaudiosync/translations";
#endif
    return m_pAppDir->path() + QDir::separator() + "translations";

}
}
