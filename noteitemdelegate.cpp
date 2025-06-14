#include "NoteItemDelegate.h"

#include <QPainter>
#include <QLinearGradient>
#include <QRandomGenerator>
#include <QApplication>

void NoteItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    QRect rect = option.rect;
    QString title = index.data(Qt::DisplayRole).toString();

    QColor color;
    int hue = qHash(title) % 360;
    color.setHsv(hue, 60, 230);

    QColor border = QColor("#634c34");
    QRect bookRect = rect.adjusted(8, 4, -8, -28);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(border);
    painter->setBrush(color);
    painter->drawRoundedRect(bookRect, 8, 8);

    painter->setPen(QPen(border.darker(150), 2));
    painter->drawLine(bookRect.left() + 8, bookRect.top() + 8, bookRect.right() - 8, bookRect.top() + 8);
    painter->drawLine(bookRect.left() + 8, bookRect.bottom() - 8, bookRect.right() - 8, bookRect.bottom() - 8);

    QFont font = option.font;
    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() * 0.9);
    painter->setFont(font);
    painter->setPen(Qt::black);

    QRect textRect = bookRect.adjusted(6, 8, -6, -8);
    QTextOption textOption(Qt::AlignCenter);
    textOption.setWrapMode(QTextOption::WordWrap);
    painter->drawText(textRect, title, textOption);

    // Полка на всю ширину виджета
    int row = index.row();
    int itemsPerRow = 4;
    int shelfHeight = 10;
    int shelfY = rect.bottom() - 6;

    if ((row + 1) % itemsPerRow == 0 || row == index.model()->rowCount() - 1) {
        QRect fullShelfRect(option.widget->rect().left(), shelfY, option.widget->width(), shelfHeight);
        QLinearGradient shelfGradient(fullShelfRect.topLeft(), fullShelfRect.bottomLeft());
        shelfGradient.setColorAt(0.0, QColor("#c58c55"));
        shelfGradient.setColorAt(1.0, QColor("#8c5a2b"));

        painter->setPen(Qt::NoPen);
        painter->setBrush(shelfGradient);
        painter->drawRoundedRect(fullShelfRect, 4, 4);
    }

    painter->restore();
}

QSize NoteItemDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const {
    return QSize(90, 130);
}
