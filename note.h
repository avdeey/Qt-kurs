#ifndef NOTE_H
#define NOTE_H

#include <QString>
#include <QColor>

struct Note {
    QString title;
    QString date;
    QString text;
    QColor textColor;
    bool isDeleted = false;
};


#endif // NOTE_H
