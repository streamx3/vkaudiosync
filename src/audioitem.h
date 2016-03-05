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

#ifndef AUDIOMODEL_H
#define AUDIOMODEL_H

#include <QString>
#include <QFile>
#include <QUrl>
#include <QObject>
#include <QRegExp>

namespace VkAudioSync
{
class AudioItem
{
private:
    QString m_aid;
    QString m_owner;
    QString m_artist;
    QString m_title;
    QUrl m_url;
    QString m_path;

    unsigned short m_progress;
    unsigned short m_status ;
    unsigned short m_duration;

    void filterStr(QString *pName);

public:
    static const unsigned short MAX_STR_LEN     = 50;
    static const int STATUS_NOTSYNCHNIZE = 1;
    static const int STATUS_NEEDUPLOAD   = 2;
    static const int STATUS_NEEDDOWNLOAD = 3;
    static const int STATUS_SYNCHRONIZED = 4;
    static const int STATUS_UNDEFINED    = 5;

    AudioItem();
    ~AudioItem();

QString aid() const;
void setAid(QString newAid);

QString owner() const;
void setOwner(QString newOwner);

QString artist() const;
void setArtist(QString newArtist);

QString title() const;
void setTitle(QString newTitle);

unsigned short duration() const;
void setDuration(unsigned short newDuration);

QUrl url() const;
void setUrl(QUrl newUrl);

QString path() const;
void setPath(QString file);

unsigned short status() const;
void setStatus(unsigned short status);

unsigned short progress() const;
void setProgress(unsigned short progress);

};
}
#endif // AUDIOMODEL_H
