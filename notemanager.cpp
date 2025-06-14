#include "NoteManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

void NoteManager::addNote(const QString &category, const Note &note) {
    categories[category].append(note);
}

void NoteManager::updateNote(const QString &category, int index, const Note &note) {
    if (categories.contains(category) && index >= 0 && index < categories[category].size()) {
        categories[category][index] = note;
    }
}

void NoteManager::deleteNote(const QString &category, int index) {
    if (categories.contains(category) && index >= 0 && index < categories[category].size()) {
        categories[category].removeAt(index);
    }
}

QVector<Note> &NoteManager::getNotes(const QString &category) {
    return categories[category];  // <-- ССЫЛКА на вектор заметок
}

QStringList NoteManager::getCategories() const {
    return categories.keys();
}

void NoteManager::addCategory(const QString &category) {
    if (!categories.contains(category)) {
        categories[category] = QVector<Note>();
    }
}

void NoteManager::deleteCategory(const QString &category) {
    categories.remove(category);
}

void NoteManager::sortNotesByDate(const QString &category) {
    if (categories.contains(category)) {
        std::sort(categories[category].begin(), categories[category].end(),
                  [](const Note &a, const Note &b) {
                      return QDate::fromString(a.date, "dd.MM.yyyy") < QDate::fromString(b.date, "dd.MM.yyyy");
                  });
    }
}

void NoteManager::loadFromJson(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();

    categories.clear();
    for (const QString &category : root.keys()) {
        QJsonArray array = root[category].toArray();
        QVector<Note> notes;
        for (const QJsonValue &val : array) {
            QJsonObject obj = val.toObject();
            Note note;
            note.title = obj["title"].toString();
            note.date = obj["date"].toString();
            note.text = obj["text"].toString();
            note.textColor = QColor(obj["textColor"].toString());
            note.isDeleted = obj["isDeleted"].toBool();
            notes.append(note);
        }
        categories[category] = notes;
    }
}

void NoteManager::saveToJson(const QString &filePath) const {
    QJsonObject root;
    for (const QString &category : categories.keys()) {
        QJsonArray array;
        for (const Note &note : categories[category]) {
            QJsonObject obj;
            obj["title"] = note.title;
            obj["date"] = note.date;
            obj["text"] = note.text;
            obj["textColor"] = note.textColor.name();
            obj["isDeleted"] = note.isDeleted;
            array.append(obj);
        }
        root[category] = array;
    }

    QJsonDocument doc(root);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}
