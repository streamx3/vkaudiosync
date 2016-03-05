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

#include "audioitemdelegate.h"

namespace VkAudioSync
{
AudioItemDelegate::AudioItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void AudioItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    int x = option.rect.x() -1;
    int y = option.rect.y() -1;
    int w = option.rect.width() -1;
    int h = option.rect.height()-1;

    QString artistStr = index.data(AudioListModel::ROLE_ARTIST).toString();
    QString titleStr  = index.data(AudioListModel::ROLE_TITLE).toString();
    unsigned short status   = index.data(AudioListModel::ROLE_STATUS).toInt();
    unsigned short progress = index.data(AudioListModel::ROLE_PROGRESS).toInt();
    int duration = index.data(AudioListModel::ROLE_DURATION).toInt(); // minutes

    int mins = qFloor(duration / 60);
    int secs = qCeil(duration - (mins * 60));
    QString minsStr;
    QString secsStr;
    if (mins < 10) {
        minsStr.append("0");
    }
    minsStr.append(QString::number(mins));

    if (secs < 10) {
        secsStr.append("0");
    }
    secsStr.append(QString::number(secs));

    QString durationStr(QString(" (%1:%2)").arg(minsStr).arg(secsStr));

    QRect selectedRect(x,y,w,h);

    if (option.state & QStyle::State_MouseOver) {
        painter->fillRect(selectedRect,QColor("#E1E7ED"));
    } else {
        painter->fillRect(selectedRect, QColor("#ffffff"));
    }

    QFont font = option.font;
    QFontMetrics  fm = QFontMetrics(font);
    int leading = fm.leading();

    QPen pen(Qt::SolidLine);

    qreal height = 0;

    QString imageSrc;
    if (status == AudioItem::STATUS_NOTSYNCHNIZE) {
        imageSrc = ":/famfam/icons/cross.png";
    } else if (status == AudioItem::STATUS_SYNCHRONIZED) {
        imageSrc = ":/famfam/icons/tick.png";
    } else {
        imageSrc = ":/famfam/icons/arrow_refresh.png";
    }
    painter->drawImage(x+5, y+h/3, QImage(imageSrc));

    int drawTextFromX = 30;

    int artistLineWidth = 30 * w / 100;

    //draw artist
    pen.setColor(QColor("#2B587A"));
    painter->setPen(pen);

    font.setPixelSize(11);
    font.setFamily("tahoma,arial,verdana,sans-serif,Lucida Sans");
    font.setBold(true);

    QTextLayout artist;
    artist.setFont(font);
    artist.setText(artistStr);
    artist.beginLayout();
        QTextLine line = artist.createLine();
        line.setLineWidth(artistLineWidth);
        height += leading;
        line.setPosition(QPointF(0, y+height));
    artist.endLayout();

    artist.draw(painter,QPoint(drawTextFromX,h/3));
    //draw title
    pen.setColor(Qt::black);
    painter->setPen(pen);
    font.setBold(false);
    int titleLineWidth = 30 * w / 100;

    QTextLayout title;
    title.setFont(font);
    title.setText(titleStr);
    title.beginLayout();
        line = title.createLine();
        line.setLineWidth(titleLineWidth);
        height += leading;
        line.setPosition(QPointF(0, y+height));
    title.endLayout();
    title.draw(painter,QPoint(drawTextFromX + artistLineWidth ,h/3));
    int durationLineWidth = 5 * w / 100;

    QTextLayout durationText;
    durationText.setFont(font);
    durationText.setText(durationStr);
    durationText.beginLayout();
        line = durationText.createLine();
        line.setLineWidth(durationLineWidth);
        height += leading;
        line.setPosition(QPointF(0, y+height));
    durationText.endLayout();
    durationText.draw(painter,QPoint(drawTextFromX + artistLineWidth + titleLineWidth ,h/3));

    int statusLineWidth = 20 * w / 100;

    if (progress > 0 && progress < 100 && status != AudioItem::STATUS_SYNCHRONIZED) {
        unsigned int progerssWidth = 20 * w / 100;
        unsigned int progressHeight = h/2;
        QRect progressBarRect(w - progerssWidth - 15 ,
                              y + (h - progressHeight)/2,
                              progerssWidth,
                              progressHeight);

        QString progressText;
        progressText.append(QString().number(progress));
        progressText.append("%");

        QStyleOptionProgressBar opt;
        opt.rect = progressBarRect;
        opt.minimum = 0;
        opt.maximum = 100;
        opt.progress = progress;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &opt, painter, 0);

        unsigned int textWidth = option.fontMetrics.width(progressText);
        unsigned int textHeight = option.fontMetrics.height();

        QRect textRect(progressBarRect.x() + (progressBarRect.width() - textWidth) / 2,
                       progressBarRect.y() + (progressBarRect.height() - textHeight) / 2,
                       textWidth,
                       textHeight);
        painter->drawText(textRect,progressText);
    } else {
        QString statusStr;
        switch (status)
        {
        case AudioItem::STATUS_NEEDDOWNLOAD:
            statusStr = tr("Need Download");
            break;

        case AudioItem::STATUS_NEEDUPLOAD:
            statusStr = tr("Need Upload");
            break;

        case AudioItem::STATUS_NOTSYNCHNIZE:
            statusStr = tr("Not Synchronized");
            break;

        case AudioItem::STATUS_SYNCHRONIZED:
            statusStr = tr("Complete");
            break;

        case AudioItem::STATUS_UNDEFINED:
        default:
            break;
        }

        QTextLayout statusText;
        statusText.setFont(font);
        statusText.setText(statusStr);
        statusText.beginLayout();
            line = statusText.createLine();
            line.setLineWidth(statusLineWidth);
            height += leading;
            line.setPosition(QPointF(0, y+height));
        statusText.endLayout();
        statusText.draw(painter, QPoint( artistLineWidth + titleLineWidth + statusLineWidth, h/3));

    }

    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->drawLine(x, y+h, x+w, y+h);
}

QSize AudioItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    if (!index.isValid())
        return QSize();

    return QSize(option.rect.width(),40);
}
}
