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

#include "vkservice.h"

namespace VK
{

VKService::VKService (QWidget *parent /*=0*/) : QObject(parent)
{
    m_redirectUri.setUrl("https://api.vk.com/blank.html");
    m_webView = new QWebView;
    m_networkManager = new QNetworkAccessManager(this);

    m_webView->resize(640,420);
    int width = QApplication::desktop()->width();
    int height = QApplication::desktop()->height();
    m_webView->move((width - m_webView->width()) / 2 , (height - m_webView->height()) / 3);

    m_authUrl.setUrl("https://api.vk.com/oauth/authorize");
    m_authUrlQuery.addQueryItem("client_id", QString::number(VK_APPLICATION_ID));
    m_authUrlQuery.addQueryItem("scope","audio,ads");
    m_authUrlQuery.addQueryItem("redirect_uri",m_redirectUri.toString());
    m_authUrlQuery.addQueryItem("display","popup");
    m_authUrlQuery.addQueryItem("response_type","token");
    m_authUrl.setQuery(m_authUrlQuery);

    m_errorHandled = false;
    m_isNeedLoadAbs = false;

    connect(m_webView, SIGNAL(urlChanged(QUrl)),
        this, SLOT(slotUrlChanged(QUrl)));
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(slotReplyFinished(QNetworkReply*)));
    connect(m_webView, SIGNAL(loadFinished(bool)),
        this, SLOT(slotLoadFinished(bool)));
}

VKService::~VKService()
{
    delete m_webView;
    delete m_networkManager;
}

void VKService::setApplicationId(QString appId)
{
    m_appId = appId;
}

void VKService::login(bool show /*= true*/)
{
    if (show) {
        m_webView->setHtml(tr("Wait..."));
        m_webView->show();
    }
    m_webView->load(m_authUrl);
    emit opened();
}

void VKService::loadAudioList()
{
    if (m_lastError.isEmpty() && !m_token.isEmpty() && !m_expire.isEmpty()) {
        QUrl url("https://api.vk.com/method/audio.get.xml");
        QUrlQuery urlq;
        urlq.addQueryItem("access_token",m_token);
        urlq.addQueryItem("redirect_uri",m_redirectUri.toString());
        url.setQuery(urlq);
        QNetworkRequest request(url);
        m_networkManager->get(request);
    }
}

void VKService::loadProfile()
{
    if (m_lastError.isEmpty() && !m_token.isEmpty() && !m_expire.isEmpty() && !m_uid.isEmpty()) {
        QUrl url("https://api.vk.com/method/getProfiles.xml");
        QUrlQuery urlq;
        urlq.addQueryItem("uids",m_uid);
        urlq.addQueryItem("fields","uid, first_name, last_name, photo, photo_medium");
        urlq.addQueryItem("access_token",m_token);
        urlq.addQueryItem("redirect_uri",m_redirectUri.toString());
        url.setQuery(urlq);
        QNetworkRequest request(url);
        m_networkManager->get(request);
    }
}

void VKService::slotReplyFinished(QNetworkReply *reply)
{
    const QByteArray response = reply->readAll();

    if (QString("/method/audio.get.xml") == reply->url().path()) {
        emit audioListLoaded(response);
    } else if (QString("/method/getProfiles.xml") == reply->url().path()) {
        if (isLogined()) {
            emit loginSuccess(response);
            emit profileLoaded(response);
        }
    } else if (m_absUrl.path() == reply->url().path()) {
        slotLoadAbsFinished(reply);
    }
}

void VKService::slotUrlChanged(const QUrl &url )
{
    QString urlAsString = url.toString();
    if (urlAsString.isEmpty() || url.path() == "/oauth/authorize")
        return;

    urlAsString.replace('#','?');
    QUrl changedUrl(urlAsString);
    QUrlQuery changedUrlQ(changedUrl);
    m_lastError = changedUrlQ.queryItemValue("error");
    if (m_lastError.isEmpty()) {
        m_token = changedUrlQ.queryItemValue("access_token");
        m_expire = changedUrlQ.queryItemValue("expires_in");
        m_uid = changedUrlQ.queryItemValue("user_id");
        loadProfile();
        loadAudioList();
        if (m_isNeedLoadAbs)
            loadAbs();
    } else {
        m_token.clear();
        m_expire.clear();
        emit loginUnsuccess();
        m_errorHandled = true;
    }
    m_webView->close();
    emit closed();
}

void VKService::slotLoadFinished(bool ok)
{
    if (!ok && ! m_errorHandled) {
        m_webView->close();
        m_lastError.append("connection failure");
        QMessageBox::critical(m_webView,"Connection error",tr("Connect to vk com failed"));
        emit loginUnsuccess();
    } else if (m_webView->url().path() == m_authUrl.path())
        m_webView->show();
    else if (QUrlQuery(m_webView->url()).hasQueryItem("error") || m_webView->url().path() == QString("/blank.html"))
        m_webView->hide();
     else
        m_webView->show();

    emit closed(); //TODO: not realy closed
}

bool VKService::isLogined() const
{
    return !m_expire.isEmpty() && m_lastError.isEmpty();
}

void VKService::logout()
{
    //TODO: clear cookies
}

void VKService::setCookieJar(QNetworkCookieJar *cookieJar)
{
    m_pCookieJar = cookieJar;
    m_webView->page()->networkAccessManager()->setCookieJar(m_pCookieJar);
}

void VKService::setLoadAbs(bool load/* = false*/)
{
    m_isNeedLoadAbs = load;
}

void VKService::slotLoadAbsFinished(QNetworkReply *reply)
{
    qDebug() << reply->readAll();
}

void VKService::loadAbs()
{
    m_absUrl.setUrl("https://api.vk.com/method/ads.getAccounts.xml");
    m_absUrlQuery.addQueryItem("redirect_uri",m_redirectUri.toString());
    m_absUrl.setQuery(m_absUrlQuery);

    QNetworkRequest request(m_absUrl);
    m_networkManager->get(request);
}
}
