
#include "RecentlyDeletedWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

RecentlyDeletedWindow::RecentlyDeletedWindow(RecentlyDeletedManager *manager, QWidget *parent)
    : QDialog(parent), manager(manager)
{
    setWindowTitle("Удалённые заметки");
    resize(400, 300);

    listWidget = new QListWidget(this);
    restoreButton = new QPushButton("Восстановить", this);
    deleteButton = new QPushButton("Удалить окончательно", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(restoreButton);
    buttonLayout->addWidget(deleteButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(listWidget);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(restoreButton, &QPushButton::clicked, this, &RecentlyDeletedWindow::restoreSelected);
    connect(deleteButton, &QPushButton::clicked, this, &RecentlyDeletedWindow::deleteSelected);

    populateList();
}

void RecentlyDeletedWindow::populateList() {
    listWidget->clear();
    const auto &notes = manager->getDeletedNotes();
    for (const auto &[category, note, index] : notes) {
        listWidget->addItem(category + ": " + note.title);
    }
}

void RecentlyDeletedWindow::restoreSelected() {
    int index = listWidget->currentRow();
    if (index < 0) return;

    QString category;
    Note note;
    int originalIndex;
    manager->restoreNote(index, category, note, originalIndex);
    emit noteRestored(category, note);
    populateList();
}

void RecentlyDeletedWindow::deleteSelected() {
    int index = listWidget->currentRow();
    if (index < 0) return;

    manager->deleteNote(index);
    populateList();
}
