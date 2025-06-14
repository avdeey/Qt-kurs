#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "NoteFormatter.h"
#include "NoteExporter.h"
#include "EmojiMenuBuilder.h"
#include "RecentlyDeletedManager.h"
#include "NoteItemDelegate.h"
#include "RecentlyDeletedWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QTextCursor>
#include <QColorDialog>
#include <QStandardItemModel>
#include <QTextDocumentWriter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
    ui->noteListView->setModel(model);
    ui->noteListView->setItemDelegate(new NoteItemDelegate(this));
    ui->noteListView->setViewMode(QListView::IconMode);
    ui->noteListView->setSpacing(12);
    ui->noteListView->setResizeMode(QListView::Adjust);
    ui->noteListView->setMovement(QListView::Static);
    ui->noteListView->setWrapping(true);

    ui->dateEdit->setMinimumDate(QDate::currentDate());
    ui->noteTitle->setMaxLength(20);

    ui->emojiButton->setIcon(QIcon(":/icons/emoji-icon.png"));
    ui->emojiButton->setIconSize(QSize(32, 32));
    ui->emojiButton->setToolTip("Insert emoji");
    ui->emojiButton->setPopupMode(QToolButton::InstantPopup);
    ui->emojiButton->setMenu(EmojiMenuBuilder::createEmojiMenu(this, [this](const QString &emoji) {
        insertEmoji(emoji);
    }));

    connect(ui->saveNoteButton, &QPushButton::clicked, this, &MainWindow::saveNote);
    connect(ui->sortButton, &QToolButton::clicked, this, &MainWindow::sortNotes);
    connect(ui->noteListView, &QListView::clicked, this, &MainWindow::loadNoteForEdit);
    connect(ui->deleteNoteButton, &QPushButton::clicked, this, &MainWindow::deleteNote);

    connect(ui->recentlyDeletedButton, &QPushButton::clicked, this, [this]() {
        if (!deletedWindow) {
            deletedWindow = new RecentlyDeletedWindow(&deletedManager, this);
            connect(deletedWindow, &RecentlyDeletedWindow::noteRestored, this, [this](const QString &category, const Note &note) {
                restoreNoteToOriginalPosition(category, note);
            });
        }
        deletedWindow->populateList();
        deletedWindow->show();
    });

    connect(ui->addCategoryButton, &QPushButton::clicked, this, &MainWindow::addCategory);
    connect(ui->deleteCategoryButton, &QPushButton::clicked, this, &MainWindow::deleteCategory);
    connect(ui->categoryBox, &QComboBox::currentTextChanged, this, &MainWindow::changeCategory);
    connect(ui->colorButton, &QToolButton::clicked, this, &MainWindow::changeTextColor);
    connect(ui->resetColorButton, &QPushButton::clicked, this, &MainWindow::resetTextColor);
    connect(ui->insertImageButton, &QToolButton::clicked, this, &MainWindow::insertImage);
    connect(ui->exportOdtButton, &QPushButton::clicked, this, &MainWindow::exportNoteToOdt);

    connect(ui->boldButton, &QToolButton::clicked, this, [this]() {
        NoteFormatter::toggleBold(ui->noteText);
    });

    connect(ui->italicButton, &QToolButton::clicked, this, [this]() {
        NoteFormatter::toggleItalic(ui->noteText);
    });

    manager.loadFromJson("notes.json");
    for (const QString &category : manager.getCategories())
        ui->categoryBox->addItem(category);
    refreshNoteList();
}

MainWindow::~MainWindow() {
    manager.saveToJson("notes.json");
    delete ui;
}

void MainWindow::saveNote() {
    QString title = ui->noteTitle->text();
    QString text = ui->noteText->toHtml();
    QString date = ui->dateEdit->date().toString("dd.MM.yyyy");
    QString category = ui->categoryBox->currentText();
    QColor color = NoteFormatter::getCurrentTextColor(ui->noteText);

    if (title.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заголовок не должен быть пустым.");
        return;
    }

    Note note = {title, date, text, color};
    QModelIndex index = ui->noteListView->currentIndex();

    if (index.isValid() && index.row() < visibleNoteIndexes.size()) {
        int realIndex = visibleNoteIndexes[index.row()];
        manager.updateNote(category, realIndex, note);
    } else {
        manager.addNote(category, note);
    }

    refreshNoteList();
    clearForm();
}


void MainWindow::deleteNote() {
    QModelIndex index = ui->noteListView->currentIndex();
    if (!index.isValid() || index.row() >= visibleNoteIndexes.size()) return;

    QString category = ui->categoryBox->currentText();
    int realIndex = visibleNoteIndexes[index.row()];
    Note note = manager.getNotes(category).value(realIndex);

    note.isDeleted = true;
    manager.updateNote(category, realIndex, note);

    deletedManager.addDeletedNote(category, note, realIndex);

    refreshNoteList();
    clearForm();
}

void MainWindow::restoreNoteToOriginalPosition(const QString &category, const Note &note) {
    auto &notes = manager.getNotes(category);
    for (int i = 0; i < notes.size(); ++i) {
        if (notes[i].title == note.title && notes[i].isDeleted) {
            manager.deleteNote(category, i);
            break;
        }
    }
    Note restored = note;
    restored.isDeleted = false;
    manager.addNote(category, restored);
    refreshNoteList();
}

void MainWindow::addCategory() {
    bool ok;
    QString name = QInputDialog::getText(this, "Новая категория", "Название:", QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty()) {
        if (name.length() > 20) {
            QMessageBox::warning(this, "Ошибка", "Название категории не должно превышать 20 символов.");
            return;
        }
        manager.addCategory(name);
        ui->categoryBox->addItem(name);
    }
}

void MainWindow::deleteCategory() {
    QString category = ui->categoryBox->currentText();
    manager.deleteCategory(category);
    ui->categoryBox->removeItem(ui->categoryBox->currentIndex());
    refreshNoteList();
}

void MainWindow::changeCategory(const QString &) {
    refreshNoteList();
}

void MainWindow::refreshNoteList() {
    model->clear();
    visibleNoteIndexes.clear();

    QString category = ui->categoryBox->currentText();
    const auto &notes = manager.getNotes(category);

    for (int i = 0; i < notes.size(); ++i) {
        if (!notes[i].isDeleted) {
            auto *item = new QStandardItem(notes[i].title);
            item->setEditable(false);
            model->appendRow(item);
            visibleNoteIndexes.append(i);
        }
    }
}

void MainWindow::clearForm() {
    ui->noteTitle->clear();
    ui->noteText->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->noteListView->clearSelection();
}

void MainWindow::loadNoteForEdit(const QModelIndex &index) {
    if (!index.isValid() || index.row() >= visibleNoteIndexes.size()) return;

    QString category = ui->categoryBox->currentText();
    int realIndex = visibleNoteIndexes[index.row()];
    Note note = manager.getNotes(category).value(realIndex);

    ui->noteTitle->setText(note.title);
    ui->dateEdit->setDate(QDate::fromString(note.date, "dd.MM.yyyy"));
    ui->noteText->setHtml(note.text);
    NoteFormatter::applyTextColor(ui->noteText, note.textColor);
}

void MainWindow::sortNotes() {
    QString category = ui->categoryBox->currentText();
    manager.sortNotesByDate(category);
    refreshNoteList();
}

void MainWindow::changeTextColor() {
    QColor color = QColorDialog::getColor(Qt::black, this);
    if (color.isValid()) {
        NoteFormatter::applyTextColor(ui->noteText, color);
    }
}

void MainWindow::resetTextColor() {
    NoteFormatter::resetTextColor(ui->noteText);
}

void MainWindow::insertImage() {
    QString path = QFileDialog::getOpenFileName(this, "Выберите изображение", "", "Images (*.png *.jpg *.bmp)");
    if (!path.isEmpty()) {
        NoteFormatter::insertImage(ui->noteText, path);
    }
}

void MainWindow::insertEmoji(const QString &emoji) {
    NoteFormatter::insertEmoji(ui->noteText, emoji);
}

void MainWindow::exportNoteToOdt() {
    QString title = ui->noteTitle->text();
    if (title.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Название заметки не должно быть пустым.");
        return;
    }
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить как ODT", title + ".odt", "ODT files (*.odt)");
    if (filePath.isEmpty()) return;
    if (NoteExporter::exportToOdt(ui->noteText->document(), filePath)) {
        QMessageBox::information(this, "Успех", "Файл успешно сохранён.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл.");
    }
}
