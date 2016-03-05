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

#include "downloadthread.h"

namespace VkAudioSync
{
DownloadThread::DownloadThread(QObject *parent) :
    QThread(parent)
{
}

DownloadThread::~DownloadThread()
{
}

void DownloadThread::setAudioListModel(AudioListModel *model)
{
    m_pAudioListModel = model;
}

void DownloadThread::run()
{
    if (m_queue.isEmpty())
        quit();

    m_needQuit = false;

    QNetworkAccessManager *networkManager = new QNetworkAccessManager;
    QNetworkReply *reply;
    QString newFileName;

    m_file = new QFile;
    QString tmpName;
    while (!m_queue.isEmpty()) {
        if (m_needQuit)
            exit();

        m_target = dequeue();
        m_name = QString("%1 - %2.mp3")
                .arg(m_pAudioListModel->data(m_target, AudioListModel::ROLE_ARTIST).toString())
                .arg(m_pAudioListModel->data(m_target, AudioListModel::ROLE_TITLE).toString());

        tmpName = QString("%1.%2")
                .arg(m_name)
                .arg("tmp");

        m_file->setFileName(m_dir->path() + QDir::separator() + tmpName);

        if (m_file->open(QIODevice::WriteOnly)) {
            QNetworkRequest request;
            request.setUrl(m_pAudioListModel->data(m_target, AudioListModel::ROLE_URL).toUrl());
            reply = networkManager->get(request);

            QEventLoop loop;
            connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
                    this,SLOT(downloadProgress(qint64,qint64)));
            connect(reply, SIGNAL(finished()),
                    &loop, SLOT(quit()));

            loop.exec();

            if (reply->error() == QNetworkReply::NoError && !m_needQuit) {
                m_file->write(reply->readAll());
                newFileName = m_dir->path() + QDir::separator() + m_name;
                if (!m_file->rename(newFileName))
                    qDebug() << "rename error " << newFileName;
                m_file->close();
                m_pAudioListModel->setData(
                            m_target,
                            QVariant(AudioItem::STATUS_SYNCHRONIZED),
                            AudioListModel::ROLE_STATUS
                );
            } else
                m_file->remove();

            syncService()->slotModelItemChanged();
        }
    }
    delete m_file;
    delete networkManager;
}

void DownloadThread::setDir(QDir *dir)
{
    m_dir = dir;
}

void DownloadThread::downloadProgress( qint64 bytesReceived, qint64 bytesTotal)
{
    if (m_needQuit)
        exit();

    unsigned short percent = qCeil(bytesReceived * 100 / bytesTotal);
    unsigned short progress = m_pAudioListModel->data(m_target, AudioListModel::ROLE_PROGRESS).toInt();
    if (progress != percent)
        m_pAudioListModel->setData(m_target, QVariant(percent), AudioListModel::ROLE_PROGRESS);

}

void DownloadThread::enqueue(const QModelIndex &index)
{
    m_queue.enqueue(index);
}

QModelIndex DownloadThread::dequeue()
{
    return m_queue.dequeue();
}

const QQueue<QModelIndex>* DownloadThread::queue() const
{
    return &m_queue;
}

void DownloadThread::slotFinished()
{
    m_pAudioListModel->setData(m_target, QVariant(100), AudioListModel::ROLE_PROGRESS);
    m_pAudioListModel->setData(m_target, QVariant(AudioItem::STATUS_SYNCHRONIZED), AudioListModel::ROLE_STATUS);
    quit();
}

void DownloadThread::stopSync()
{
    m_needQuit = true;
    if (m_queue.isEmpty())
        quit();
}

void DownloadThread::setSyncService(SynchService* pSyncService)
{
    m_pSyncService = pSyncService;
}

SynchService* DownloadThread::syncService() const
{
    return m_pSyncService;
}
}
