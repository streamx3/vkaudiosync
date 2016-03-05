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

#ifndef PROFILEMODEL_H
#define PROFILEMODEL_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QByteArray>
#include <QtXml>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace VkAudioSync
{
class ProfileModel : public QObject
{
    Q_OBJECT

private:
    QString m_uid;
    QString m_firstName;
    QString m_lastName;
    QUrl m_photoUrl;
    QUrl m_photoMediumUrl;
    QImage m_photo;
    QImage m_photoMedium;
    QNetworkAccessManager *m_pNetworkAccessManager;

public:
    explicit ProfileModel(QObject* parent = 0);
    ~ProfileModel();
    void  setUid(const QString &uid);
    QString uid() const;
    void setFirstName(const QString &firstName);
    QString firsrtName() const;
    void setLastName(const QString &lastName);
    QString lastName() const;
    void setPhotoUrl(const QUrl &url);
    QUrl photoUrl() const;
    void setPhotoMediumUrl(const QUrl &url);
    QUrl photoMediumUrl() const;
    QImage photo() const;
    QImage photoMedium() const;
    void parseXml(const QByteArray &xml);

protected slots:
    void slotFinished(QNetworkReply* reply);

signals:
    void photoLoaded();
    void photoMediumLoaded();
};
}
#endif // PROFILEMODEL_H
