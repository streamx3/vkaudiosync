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

#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include <QObject>
#include <QSettings>
#include <QDir>
#include <QVector>
#include <QWaitCondition>
#include <QMutex>

#include "downloadthread.h"
#include "audiolistmodel.h"

#ifndef THREADS
#define THREADS 6
#endif

namespace VkAudioSync
{
class DownloadThread;
class SynchService : public QObject
{
    Q_OBJECT

private:
    QDir *m_dir;
    unsigned short m_theadsCount;
    AudioListModel *m_pAudioListModel;
    DownloadThread *m_pDownloadThreads[THREADS];

public:
    explicit SynchService(QObject *parent = 0);
    ~SynchService();
    void setDir(QDir *pdir);
    const QDir* dir() const;
    void setThreadsCount(unsigned short count);
    unsigned short threadsCount() const;
    void setAudioModel(AudioListModel *pAudioListModel);
    void synchronize();
    void setStatuses();
    void removeTmpFiles();
    void stopSync();

public slots:
    void synchFinished();
    void slotModelItemChanged();

signals:
    void synchronizeFinished(bool);
    void updateStatusBar(const QString);
};
}
#endif // SYNCHRONIZER_H
