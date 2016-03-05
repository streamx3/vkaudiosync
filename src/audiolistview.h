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

#ifndef AUDIOLISTVIEW_H
#define AUDIOLISTVIEW_H

#include <QListView>
#include <QMouseEvent>

#include "audiolistmodel.h"

namespace VkAudioSync
{
class AudioListView : public QListView
{
    Q_OBJECT
public:
    explicit AudioListView(QWidget *parent = 0);

protected:
    virtual void mouseReleaseEvent ( QMouseEvent * e );

};
}
#endif // AUDIOLISTVIEW_H
