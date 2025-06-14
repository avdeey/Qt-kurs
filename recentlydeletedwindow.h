#ifndef RECENTLYDELETEDWINDOW_H
#define RECENTLYDELETEDWINDOW_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include "RecentlyDeletedManager.h"

class RecentlyDeletedWindow : public QDialog {
    Q_OBJECT

public:
    RecentlyDeletedWindow(RecentlyDeletedManager *manager, QWidget *parent = nullptr);
    void populateList();  // делаем публичным для повторного вызова

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
};

#endif // RECENTLYDELETEDWINDOW_H
