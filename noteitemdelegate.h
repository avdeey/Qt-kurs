#ifndef NOTEITEMDELEGATE_H
#define NOTEITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class NoteItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // NOTEITEMDELEGATE_H
