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

#include "audioitem.h"

namespace VkAudioSync
{
AudioItem::AudioItem()
{
    m_status = STATUS_UNDEFINED;
    m_progress = 0;
}

AudioItem::~AudioItem()
{
}

QString AudioItem::aid() const
{
    return m_aid;
}

void AudioItem::setAid(QString newAid)
{
    m_aid = newAid;
}

QString AudioItem::owner() const
{
    return m_owner;
}

void AudioItem::setOwner(QString newOwner)
{
    m_owner = newOwner;
    filterStr(&m_owner);
}

QString AudioItem::artist() const
{
    return m_artist;
}

void AudioItem::setArtist(QString newArtist)
{
    m_artist = newArtist;
    filterStr(&m_artist);
}

QString AudioItem::title() const
{
    return m_title;
}

void AudioItem::setTitle(QString newTitle)
{
    m_title = newTitle;
    filterStr(&m_title);
}

unsigned short AudioItem::duration() const
{
    return m_duration;
}

void AudioItem::setDuration(unsigned short newDuration)
{
    m_duration = newDuration;
}

QUrl AudioItem::url() const
{
    return m_url;
}

void AudioItem::setUrl(QUrl newUrl)
{
    m_url = newUrl;
}

unsigned short AudioItem::status() const
{
    return m_status;
}

void AudioItem::setStatus(unsigned short status)
{
    m_status = status;
}

QString AudioItem::path() const
{
    return m_path;
}

void AudioItem::setPath(QString file)
{
    m_path = file;
}

unsigned short AudioItem::progress() const
{
    return m_progress;
}

void AudioItem::setProgress(unsigned short progress)
{
    m_progress = progress;
}

void AudioItem::filterStr(QString *pName)
{
    QRegExp regexp("[^\\d,\\w,\\-,\\s,\\(,\\),\\,]+");
    pName->replace(regexp,"");

    if (pName->length() > MAX_STR_LEN)
        *pName = pName->left(MAX_STR_LEN);
}
}
