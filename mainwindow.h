#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QPlainTextEdit>
#include <QListWidget>
#include <QDateEdit>
#include <QVector>
#include <QComboBox>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    struct Note {
        QString title;
        QString date;
        QString text;
    };
private slots:
    void addNewNote();
    void saveToCSV();
    void loadNoteForEdit(QListWidgetItem *item);
    void deleteNote();
    void addCategory();
    void deleteCategory();
    void changeCategory(const QString &category);


private:
    QLineEdit *noteTitle;
    QDateEdit *dateEdit;
    QPlainTextEdit *noteText;
    QListWidget *noteList;
    QPushButton *addNewNoteButton;
    QPushButton *saveCSVButton;
    QPushButton *deleteNoteButton;
    QPushButton *addCategoryButton;
    QPushButton *deleteCategoryButton;
    QComboBox *categoryBox;


    QMap<QString, QVector<Note>> categorizedNotes; // Категория -> список заметок
    void refreshNoteList();
};

#endif // MAINWINDOW_H
