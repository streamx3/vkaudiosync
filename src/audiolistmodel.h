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

#ifndef AUDIOLISTMODEL_H
#define AUDIOLISTMODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include <QList>
#include <QByteArray>
#include <QtXml>
#include <QtAlgorithms>

#include "audioitem.h"

namespace VkAudioSync
{
class AudioListModel : public QAbstractListModel
{
    Q_OBJECT

private:
    int m_sortBy;
    QList<AudioItem> *m_pItems;
    static inline bool sortCmpByArtist(const AudioItem &a, const AudioItem &b);
    static inline bool sortCmpByTitle(const AudioItem &a, const AudioItem &b);
    static inline bool sortCmpByDuration(const AudioItem &a, const AudioItem &b);
    static inline bool sortCmpByProgress(const AudioItem &a, const AudioItem &b);
    static inline bool sortCmpByStatus(const AudioItem &a, const AudioItem &b);

public:
    static const int SORT_UNDEFINED = 0;
    static const int SORT_ARTIST    = 1;
    static const int SORT_NAME      = 2;
    static const int SORT_DURATION  = 3;
    static const int SORT_PROGRESS  = 4;
    static const int SORT_STATUS    = 5;

    static const int ROLE_ARTIST   = 33;
    static const int ROLE_TITLE    = 34;
    static const int ROLE_DURATION = 35;
    static const int ROLE_STATUS   = 36;
    static const int ROLE_PROGRESS = 37;
    static const int ROLE_URL	   = 38;

    explicit AudioListModel(QObject *parent = 0);
    virtual ~AudioListModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual void resetStatuses();
    virtual const QString statusRow() const;
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    void parseXml(const QByteArray &xml);
    void clear();
    void setSortBy(int sort);
    int sortBy();
};
}
#endif // AUDIOLISTMODEL_H
