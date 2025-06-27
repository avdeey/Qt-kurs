#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "NoteFormatter.h"
#include "NoteExporter.h"
#include "EmojiMenuBuilder.h"
#include "RecentlyDeletedManager.h"
#include "NoteItemDelegate.h"
#include "RecentlyDeletedWindow.h"

#include <QTemporaryDir>
#include <QXmlStreamReader>
#include <QBuffer>



#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QTextCursor>
#include <QColorDialog>
#include <QStandardItemModel>
#include <QTextDocumentWriter>
#include <QDate>
#include <QDebug>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), sortAscending(true) {
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
           deletedWindow = new RecentlyDeletedWindow(&deletedManager, &manager, this);
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

    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::searchNotes);
    connect(ui->renameCategoryButton, &QPushButton::clicked, this, &MainWindow::renameCategory);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::clearForm);

    manager.loadFromJson("notes.json");
    for (const QString &category : manager.getCategories())
        ui->categoryBox->addItem(category);
    refreshNoteList();
}

MainWindow::~MainWindow() {
    manager.saveToJson("notes.json");
    delete ui;
}

void MainWindow::refreshNoteList() {
    model->clear();
    QString category = ui->categoryBox->currentText();
    const auto &notes = manager.getNotes(category);

    for (const Note &note : notes) {
        if (!note.isDeleted) {
            auto *item = new QStandardItem(note.title);
            item->setEditable(false);
            model->appendRow(item);
        }
    }
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

    Note newNote = {title, date, text, color};
    QVector<Note> &notes = manager.getNotes(category);
    bool updated = false;
    for (int i = 0; i < notes.size(); ++i) {
        if (notes[i].title == title && !notes[i].isDeleted) {
            manager.updateNote(category, i, newNote);
            updated = true;
            break;
        }
    }
    if (!updated) manager.addNote(category, newNote);

    refreshNoteList();
    clearForm();
}

void MainWindow::deleteNote() {
    QModelIndex index = ui->noteListView->currentIndex();
    if (!index.isValid()) return;

    QString category = ui->categoryBox->currentText();
    QString selectedTitle = index.data().toString();
    QVector<Note> &notes = manager.getNotes(category);
    for (int i = 0; i < notes.size(); ++i) {
        if (notes[i].title == selectedTitle && !notes[i].isDeleted) {
            notes[i].isDeleted = true;
            deletedManager.addDeletedNote(category, notes[i], i);
            break;
        }
    }
    refreshNoteList();
    clearForm();
}

void MainWindow::loadNoteForEdit(const QModelIndex &index) {
    if (!index.isValid()) return;
    QString category = ui->categoryBox->currentText();
    QString selectedTitle = index.data().toString();
    const auto &notes = manager.getNotes(category);
    for (const Note &note : notes) {
        if (note.title == selectedTitle && !note.isDeleted) {
            ui->noteTitle->setText(note.title);
            ui->dateEdit->setDate(QDate::fromString(note.date, "dd.MM.yyyy"));
            ui->noteText->setHtml(note.text);
            NoteFormatter::applyTextColor(ui->noteText, note.textColor);
            break;
        }
    }
}

void MainWindow::sortNotes() {
    QString category = ui->categoryBox->currentText();
    manager.sortNotesByDate(category, sortAscending);
    refreshNoteList();
    sortAscending = !sortAscending;
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


void MainWindow::clearForm() {
    ui->noteTitle->clear();
    ui->noteText->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->noteListView->clearSelection();
    NoteFormatter::resetTextColor(ui->noteText);
    QTextCursor cursor = ui->noteText->textCursor();
    QTextCharFormat format;
    format.setFontWeight(QFont::Normal);
    format.setFontItalic(false);
    cursor.mergeCharFormat(format);
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
    manager.deleteCategory(category, deletedManager);
    ui->categoryBox->removeItem(ui->categoryBox->currentIndex());
    refreshNoteList();
}

void MainWindow::changeCategory(const QString &) {
    ui->lineEdit->clear();
    refreshNoteList();
}

void MainWindow::renameCategory() {
    QString oldCategory = ui->categoryBox->currentText();
    if (oldCategory.isEmpty()) return;

    bool ok;
    QString newCategory = QInputDialog::getText(this, "Изменить категорию",
                                                "Новое название категории:",
                                                QLineEdit::Normal, oldCategory, &ok);
    if (!ok || newCategory.isEmpty() || newCategory == oldCategory)
        return;
    if (newCategory.length() > 20) {
        QMessageBox::warning(this, "Ошибка", "Название категории не должно превышать 20 символов.");
        return;
    }
    if (manager.getCategories().contains(newCategory)) {
        QMessageBox::warning(this, "Ошибка", "Категория с таким названием уже существует.");
        return;
    }
    manager.renameCategory(oldCategory, newCategory);
    if (deletedWindow) {
        deletedWindow->renameCategory(oldCategory, newCategory);
    }
    int index = ui->categoryBox->currentIndex();
    ui->categoryBox->setItemText(index, newCategory);
    refreshNoteList();
}

void MainWindow::searchNotes(const QString &text) {
    model->clear();
    QString category = ui->categoryBox->currentText();
    const auto &notes = manager.getNotes(category);
    for (const Note &note : notes) {
        if (!note.isDeleted && note.title.contains(text, Qt::CaseInsensitive)) {
            auto *item = new QStandardItem(note.title);
            item->setEditable(false);
            model->appendRow(item);
        }
    }
}

void MainWindow::restoreNoteToOriginalPosition(const QString &category, const Note &note) {
    auto &notes = manager.getNotes(category);

    // Удалим дубликаты перед восстановлением
    for (int i = 0; i < notes.size(); ++i) {
        if (notes[i].title == note.title) {
            notes.removeAt(i);
            break;
        }
    }

    Note restored = note;
    restored.isDeleted = false;
    manager.addNote(category, restored);

    if (ui->categoryBox->findText(category) == -1) {
        ui->categoryBox->addItem(category);
    }
    ui->categoryBox->setCurrentText(category);
    refreshNoteList();
}


void MainWindow::clearSelection() {
    ui->noteListView->clearSelection();
    ui->noteTitle->clear();
    ui->noteText->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    NoteFormatter::resetTextColor(ui->noteText);
}

