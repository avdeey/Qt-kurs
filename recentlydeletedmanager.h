#ifndef RECENTLYDELETEDMANAGER_H
#define RECENTLYDELETEDMANAGER_H

#include <QString>
#include <QVector>
#include <tuple>
#include "Note.h"

class RecentlyDeletedManager {
public:
    void addDeletedNote(const QString &category, const Note &note, int originalIndex);
    void restoreNote(int index, QString &category, Note &note, int &originalIndex);
    void deleteNote(int index);
    void clear();
    QVector<std::tuple<QString, Note, int>> getDeletedNotes() const;

private:
    QVector<std::tuple<QString, Note, int>> deletedNotes;
};

#endif // RECENTLYDELETEDMANAGER_H
