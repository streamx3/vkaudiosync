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

#ifndef AUDIOITEMDELEGATE_H
#define AUDIOITEMDELEGATE_H

#include <QPainter>
#include <QTextLayout>
#include <QTextLine>
#include <QtWidgets/QItemDelegate>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QApplication>

#include "audiolistmodel.h"

namespace VkAudioSync
{
class AudioItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit AudioItemDelegate(QObject *parent = 0);
    virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

};
}
#endif // AUDIOITEMDELEGATE_H
