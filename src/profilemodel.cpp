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

#include "profilemodel.h"

namespace VkAudioSync
{
ProfileModel::ProfileModel(QObject* parent /*= 0*/) : QObject(parent)
{
    m_pNetworkAccessManager = new QNetworkAccessManager(this);

    connect(m_pNetworkAccessManager,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotFinished(QNetworkReply*))
    );
}

ProfileModel::~ProfileModel()
{
    delete m_pNetworkAccessManager;
}

void ProfileModel::setUid(const QString &uid)
{
    m_uid = uid;
}

QString ProfileModel::uid() const
{
    return m_uid;
}

void ProfileModel::setFirstName(const QString &firstName)
{
    m_firstName = firstName;
}

QString ProfileModel::firsrtName() const
{
    return m_firstName;
}

void ProfileModel::setLastName(const QString &lastName)
{
    m_lastName = lastName;
}

QString ProfileModel::lastName() const
{
    return m_lastName;
}

void ProfileModel::setPhotoUrl(const QUrl &url)
{
    m_photoUrl = url;
    if (!m_photoUrl.isEmpty()) {
        QNetworkRequest request(m_photoUrl);
        m_pNetworkAccessManager->get(request);
    }
}

QUrl ProfileModel::photoUrl() const
{
    return m_photoUrl;
}

void ProfileModel::setPhotoMediumUrl(const QUrl &url)
{
    m_photoMediumUrl = url;
    if (!m_photoMediumUrl.isEmpty()) {
        QNetworkRequest request(m_photoMediumUrl);
        m_pNetworkAccessManager->get(request);
    }
}

QUrl ProfileModel::photoMediumUrl() const
{
    return m_photoMediumUrl;
}

void ProfileModel::parseXml(const QByteArray &xml)
{
    QString uid;
    QString firstName;
    QString lastName;
    QUrl photoUrl;
    QUrl photoMediumUrl;

    QDomDocument dom;
    dom.setContent(xml);

    QDomElement root = dom.firstChildElement(); // <response> root element

    if (root.nodeName() == "response") {
        QDomNode  audioElement = root.firstChildElement(); // <user>
        if (audioElement.nodeName() == "user") {
            QDomNodeList nodeList;
            while (!audioElement.isNull()) {
                QDomElement element = audioElement.toElement();

                nodeList = element.elementsByTagName("uid");
                if (nodeList.count() > 0) {
                    uid = nodeList.item(0).toElement().text();
                } else {
                    uid.clear();
                }

                nodeList = element.elementsByTagName("first_name");
                if (nodeList.count() > 0) {
                    firstName = nodeList.item(0).toElement().text();
                } else {
                    firstName.clear();
                }

                nodeList = element.elementsByTagName("last_name");
                if (nodeList.count() > 0) {
                    lastName = nodeList.item(0).toElement().text();
                } else {
                    lastName.clear();
                }

                nodeList = element.elementsByTagName("photo");
                if (nodeList.count() > 0) {
                    photoUrl.setUrl(nodeList.item(0).toElement().text());
                } else {
                    photoUrl.clear();
                }

                nodeList = element.elementsByTagName("photo_medium");
                if (nodeList.count() > 0) {
                    photoMediumUrl.setUrl(nodeList.item(0).toElement().text());
                } else {
                    photoMediumUrl.clear();
                }

                setUid(uid);
                setFirstName(firstName);
                setLastName(lastName);
                setPhotoUrl(photoUrl);
                setPhotoMediumUrl(photoMediumUrl);

                audioElement = audioElement.nextSibling();
            }
        }
    }
}

void ProfileModel::slotFinished(QNetworkReply* reply)
{
    QByteArray imgData = reply->readAll();
    if (reply->url().toString() == m_photoUrl.toString()) {
        m_photo.loadFromData(imgData);
        emit photoLoaded();
    } else if (reply->url().toString() == m_photoMediumUrl.toString()) {
        m_photoMedium.loadFromData(imgData);
        emit photoMediumLoaded();
    }
}

QImage ProfileModel::photo() const
{
    return m_photo;
}

QImage ProfileModel::photoMedium() const
{
    return m_photoMedium;
}
}
