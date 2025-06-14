#ifndef NOTEMANAGER_H
#define NOTEMANAGER_H

#include <QString>
#include <QMap>
#include <QVector>
#include "Note.h"

class NoteManager {
public:
    void addNote(const QString &category, const Note &note);
    void updateNote(const QString &category, int index, const Note &note);
    void deleteNote(const QString &category, int index);
    QVector<Note> &getNotes(const QString &category);  // <-- ВАЖНО: ссылка
    QStringList getCategories() const;
    void addCategory(const QString &category);
    void deleteCategory(const QString &category);
    void sortNotesByDate(const QString &category);
    void loadFromJson(const QString &filePath);
    void saveToJson(const QString &filePath) const;

private:
    QMap<QString, QVector<Note>> categories;
};

#endif // NOTEMANAGER_H
