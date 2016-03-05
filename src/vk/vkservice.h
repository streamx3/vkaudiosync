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

#ifndef VKMODEL_H
#define VKMODEL_H

#ifndef VK_APPLICATION_ID
#define VK_APPLICATION_ID 2169954
#endif

#include <QtWidgets/QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QTextCodec>

#include <QtWebKitWidgets/QWebView>
#include <QtNetwork>

#include <QString>
#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QByteArray>

namespace VK
{
class VKService : public QObject
{
    Q_OBJECT
private://
    QString m_appId;
    QString m_uid;
    QString m_token;
    QString m_expire;
    QString m_lastError;
    QUrl m_authUrl;
    QUrlQuery m_authUrlQuery;
    QUrl m_absUrl;
    QUrlQuery m_absUrlQuery;
    QUrl m_redirectUri;
    bool m_isNeedLoadAbs;
    QNetworkCookieJar *m_pCookieJar;
    QWebView *m_webView;
    QNetworkAccessManager *m_networkManager;
    bool m_errorHandled;
    void loadAudioList();
    void loadProfile();
    void loadAbs();

private slots:
    void slotUrlChanged(const QUrl &url);
    void slotReplyFinished(QNetworkReply *reply );
    void slotLoadFinished(bool ok);
    void slotLoadAbsFinished(QNetworkReply *reply);

public:
    explicit VKService(QWidget *parent = 0 );
    ~VKService();

    void setApplicationId(QString appId);
    void login(bool show = true) ;
    bool isLogined() const;
    void logout();
    void setCookieJar(QNetworkCookieJar *cookieJar);
    void setLoadAbs(bool load = false);

signals:
    void opened();
    void closed();
    void loginSuccess(const QByteArray &);
    void audioListLoaded(const QByteArray &);
    void profileLoaded(const QByteArray &);
    void loginUnsuccess();
};
}

#endif // VKMODEL_H
