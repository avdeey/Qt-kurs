#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QVector>
#include "NoteManager.h"
#include "RecentlyDeletedManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QListWidgetItem;
class NoteItemDelegate;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void saveNote();
    void deleteNote();
    void addCategory();
    void deleteCategory();
    void changeCategory(const QString &category);
    void sortNotes();
    void changeTextColor();
    void resetTextColor();
    void insertImage();
    void insertEmoji(const QString &emoji);
    void exportNoteToOdt();
    void loadNoteForEdit(const QModelIndex &index);
    void restoreNoteToOriginalPosition(const QString &category, const Note &note);
    void searchNotes(const QString &text);
    void renameCategory();
    void clearSelection();

private:
    void refreshNoteList();
    void clearForm();

    Ui::MainWindow *ui;
    QStandardItemModel *model;
    NoteManager manager;
    RecentlyDeletedManager deletedManager;
    QVector<int> visibleNoteIndexes;
    class RecentlyDeletedWindow *deletedWindow = nullptr;
    QVector<int> searchResults;
    bool sortAscending = true;
};

#endif // MAINWINDOW_H
