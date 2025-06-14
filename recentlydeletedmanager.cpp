#include "RecentlyDeletedManager.h"

void RecentlyDeletedManager::addDeletedNote(const QString &category, const Note &note, int originalIndex) {
    deletedNotes.append({category, note, originalIndex});
}

void RecentlyDeletedManager::restoreNote(int index, QString &category, Note &note, int &originalIndex) {
    if (index >= 0 && index < deletedNotes.size()) {
        std::tie(category, note, originalIndex) = deletedNotes[index];
        note.isDeleted = false;
        deletedNotes.removeAt(index);
    }
}

void RecentlyDeletedManager::deleteNote(int index) {
    if (index >= 0 && index < deletedNotes.size()) {
        deletedNotes.removeAt(index);
    }
}

void RecentlyDeletedManager::clear() {
    deletedNotes.clear();
}

QVector<std::tuple<QString, Note, int>> RecentlyDeletedManager::getDeletedNotes() const {
    return deletedNotes;
}
