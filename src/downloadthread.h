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

#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include <QThread>
#include <QQueue>
#include <QUrl>
#include <QDir>
#include <QtNetwork>
#include <QFile>
#include <QEventLoop>

#include "audiolistmodel.h"
#include "synchservice.h"

namespace VkAudioSync
{
class SynchService;
class DownloadThread : public QThread
{
    Q_OBJECT

private:
    QDir* m_dir;
    QString m_name;
    QFile* m_file;
    QQueue<QModelIndex> m_queue;
    QModelIndex m_target;
    SynchService* m_pSyncService;
    bool m_needQuit;
    AudioListModel* m_pAudioListModel;
    bool m_needWait;

    bool ready();

protected:
    virtual void run();

public:
    explicit DownloadThread(QObject *parent = 0);
    virtual ~DownloadThread();

    void setDir(QDir *dir);

    void enqueue(const QModelIndex &index);
    QModelIndex dequeue();
    const QQueue<QModelIndex>* queue() const;

    void clearQueue();
    void setAudioListModel(AudioListModel *model);
    void setSyncService(SynchService* pSyncService);
    SynchService* syncService() const;


protected slots:
    void downloadProgress( qint64 bytesReceived, qint64 bytesTotal);
    void slotFinished();

public slots:
    void stopSync();
};
}
#endif // DOWNLOADTHREAD_H
