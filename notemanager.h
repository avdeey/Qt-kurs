#ifndef NOTEMANAGER_H
#define NOTEMANAGER_H

#include <QString>
#include <QMap>
#include <QVector>
#include "Note.h"
#include "recentlydeletedmanager.h"

class NoteManager {
public:
    void addNote(const QString &category, const Note &note);
    void updateNote(const QString &category, int index, const Note &note);
    void deleteNote(const QString &category, int index);
    QVector<Note> &getNotes(const QString &category);
    QStringList getCategories() const;
    void addCategory(const QString &category);
    void deleteCategory(const QString &category, RecentlyDeletedManager &trashManager);
    void sortNotesByDate(const QString &category, bool ascending = true);
    void loadFromJson(const QString &filePath);
    void saveToJson(const QString &filePath) const;
    void renameCategory(const QString &oldName, const QString &newName);

private:
    QMap<QString, QVector<Note>> categories;
};

#endif // NOTEMANAGER_H
