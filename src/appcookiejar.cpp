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

#include "appcookiejar.h"

namespace VkAudioSync
{
AppCookieJar::AppCookieJar(QObject *parent) :
    QNetworkCookieJar(parent)
{
    m_useFile = false;
    m_needSave = false;
}

AppCookieJar::~AppCookieJar()
{
}

void AppCookieJar::setFile(QFile *pFile)
{
    m_useFile = true;
    m_pFile = pFile;
}

QFile* AppCookieJar::file() const
{
    return m_pFile;
}

QList<QNetworkCookie> AppCookieJar::cookiesForUrl ( const QUrl & url ) const
{
    if (!m_useFile)
        return QNetworkCookieJar::cookiesForUrl(url);

    bool isEncrypted = url.scheme().toLower() == "https";

    QList<QNetworkCookie> list;
    QDomElement root = m_document.firstChildElement();
    QDateTime now = QDateTime::currentDateTime();
    if (!root.isNull()) {
        QDomElement element = root.firstChildElement();
        while (!element.isNull()) {
            QNetworkCookie cookie;
            QDomElement xmlCookie = element.firstChildElement();
            bool needAdd = true;
            while (!xmlCookie.isNull()) {
                /*
                      <secure>0</secure>
                      <session>0</session>
                      <httpOnly>0</httpOnly>
                      <domain>.vkontakte.ru</domain>
                      <path>/</path>
                      <name>remixchk</name>
                      <value>5</value>
                      <expire>2013-05-28T03:20:49Z</expire>
                */

                if ("secure" == xmlCookie.tagName()) {
                    cookie.setSecure("1" == xmlCookie.text());
                } else if ("httpOnly" == xmlCookie.tagName()) {
                    cookie.setHttpOnly("1" == xmlCookie.text());
                } else if ("domain" == xmlCookie.tagName()) {
                    cookie.setDomain(xmlCookie.text());
                } else if ("path" == xmlCookie.tagName()) {
                    cookie.setPath(xmlCookie.text());
                } else if ("name" == xmlCookie.tagName()) {
                    cookie.setName(xmlCookie.text().toUtf8());
                } else if ("value" == xmlCookie.tagName()) {
                    cookie.setValue(xmlCookie.text().toUtf8());
                } else if ("expire" == xmlCookie.tagName()) {
                    QDateTime datetime;
                    datetime.fromString(xmlCookie.text(),Qt::ISODate);
                    cookie.setExpirationDate(datetime);
                }

                xmlCookie = xmlCookie.nextSiblingElement();
            }

            if (!isParentDomain(url.host(), cookie.domain()))
                needAdd = false;
            if (!isParentPath(url.path(), cookie.path()))
                needAdd = false;
            if (!cookie.isSessionCookie() && cookie.expirationDate() < now)
                needAdd = false;
            if (cookie.isSecure() && !isEncrypted)
                needAdd = false;
            if (needAdd)
                list << cookie;

            element = element.nextSiblingElement();
        }
    }

    return list;
}

inline bool AppCookieJar::isParentPath(QString path, QString reference) const
{
    if (!path.endsWith(QLatin1Char('/')))
        path += QLatin1Char('/');
    if (!reference.endsWith(QLatin1Char('/')))
        reference += QLatin1Char('/');
    return path.startsWith(reference);
}

inline bool AppCookieJar::isParentDomain(QString domain, QString reference) const
{
    if (!reference.startsWith(QLatin1Char('.')))
        return domain == reference;

    return domain.endsWith(reference) || domain == reference.mid(1);
}

inline QDomText AppCookieJar::elementByText(QDomDocument &document, const QString &str)
{
    return document.createTextNode(str);
}

void AppCookieJar::createXmlNodeByCookie(QDomElement *parent, const QNetworkCookie *cookie, const QUrl *url)
{
    QDomElement child = parent->firstChildElement();
    while (!child.isNull()) {
        parent->removeChild(child);
        child = parent->firstChildElement();
    }

    QDomElement secure = m_document.createElement("secure");
    secure.appendChild(elementByText(m_document, QString(cookie->isSecure() ? "1" : "0")));
    parent->appendChild(secure);

    QDomElement session = m_document.createElement("session");
    session.appendChild(elementByText(m_document, QString(cookie->isSessionCookie() ? "1" : "0")));
    parent->appendChild(session);

    QDomElement httpOnly = m_document.createElement("httpOnly");
    httpOnly.appendChild(elementByText(m_document, QString(cookie->isHttpOnly() ? "1" : "0")));
    parent->appendChild(httpOnly);

    QDomElement domain = m_document.createElement("domain");
    domain.appendChild(elementByText(m_document, cookie->domain()));
    parent->appendChild(domain);

    QDomElement path = m_document.createElement("path");
    path.appendChild(elementByText(m_document, cookie->path()));
    parent->appendChild(path);

    QDomElement name = m_document.createElement("name");
    name.appendChild(elementByText(m_document, cookie->name()));
    parent->appendChild(name);

    QDomElement value = m_document.createElement("value");
    value.appendChild(elementByText(m_document, cookie->value()));
    parent->appendChild(value);

    QDomElement expire = m_document.createElement("expire");
    expire.appendChild(elementByText(m_document,cookie->expirationDate().toString(Qt::ISODate)));
    parent->appendChild(expire);

    QDomElement urlElement = m_document.createElement("url");
    urlElement.appendChild(elementByText(m_document, url->toString()));
    parent->appendChild(urlElement);
}

bool AppCookieJar::setCookiesFromUrl ( const QList<QNetworkCookie> & cookieList, const QUrl & url )
{
    if (!m_useFile)
        return QNetworkCookieJar::setCookiesFromUrl(cookieList,url);

    QDomElement root;
    if (m_document.firstChild().isNull() || m_document.firstChildElement().tagName() != "cookies") {
        root = m_document.createElement("cookies");
        m_document.appendChild(root);
    } else {
        root = m_document.firstChildElement();
    }

    QNetworkCookie cookie;
    for (int i = 0; i < cookieList.size(); i++) {
        bool exist = false;
        cookie = cookieList.at(i);

        QDomElement element = root.firstChildElement();
        while (!element.isNull()) {
            QDomNodeList list = element.elementsByTagName("name");
            if (!list.isEmpty()) {
                QDomElement el = list.at(0).toElement();
                if (el.text() == cookie.name()) {
                    createXmlNodeByCookie(&element,&cookie,&url);
                    exist = true;
                    break;
                }
            }
            element = element.nextSiblingElement();
        }

        if (!exist) {
            QDomElement cookieElement = m_document.createElement("cookie");
            createXmlNodeByCookie(&cookieElement, &cookie, &url);
            root.appendChild(cookieElement);
        }
    }

    return true;
}

void AppCookieJar::clear()
{
    m_document.clear();
}

void AppCookieJar::save()
{
    if (!m_pFile->open(QIODevice::WriteOnly))
        return ;

    m_pFile->write(m_document.toString().toUtf8());
    m_pFile->close();
}

void AppCookieJar::restore()
{
    if (!m_pFile->open(QIODevice::ReadOnly))
        return ;

    QByteArray data = m_pFile->readAll();
    m_document.setContent(data);
    m_pFile->close();
}
}
