#ifndef RECENTLYDELETEDWINDOW_H
#define RECENTLYDELETEDWINDOW_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include "RecentlyDeletedManager.h"
#include "NoteManager.h"

class RecentlyDeletedWindow : public QDialog {
    Q_OBJECT

public:
    RecentlyDeletedWindow(RecentlyDeletedManager *manager, NoteManager *noteManager, QWidget *parent = nullptr);

    void populateList();
    void renameCategory(const QString &oldName, const QString &newName);

signals:
    void noteRestored(const QString &category, const Note &note);

private slots:
    void restoreSelected();
    void deleteSelected();

private:
    RecentlyDeletedManager *manager;
    QListWidget *listWidget;
    QPushButton *restoreButton;
    QPushButton *deleteButton;
    NoteManager *noteManager;
};

#endif // RECENTLYDELETEDWINDOW_H
