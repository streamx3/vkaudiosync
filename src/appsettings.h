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

#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QCoreApplication>
#include <QObject>
#include <QDir>
#include <QSettings>

//use in impl.
#include <QDesktopServices>
#include <QFile>
#include <QtXml>
#include <QVariant>

#include "appcookiejar.h"

namespace VkAudioSync
{
class AppSettings : QObject
{
Q_OBJECT

private:
    bool m_useConfig;
    QDir *m_pAppDir;
    QDir *m_pUserDir;
    QSettings *m_pSettings;
    QCoreApplication *m_pApp;
    AppCookieJar *m_pCookieJar;
    QFile *m_pCookieFile;
    void setCookieFile(QString path);

public:
    AppSettings(QObject *parent = 0);
    ~AppSettings();

    void load();
    void setValue(const QString & key, const QVariant & value );
    QVariant value ( const QString & key, const QVariant & defaultValue = QVariant() ) const;
    AppCookieJar* cookieJar() const;
    void clear();
    void clearCookies();
    void save();
    void restore();

    QDir* applicationPath();
    QDir* userPath();
    QString translationPath();
};
}
#endif // MYSETTINGS_H
