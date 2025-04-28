#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include <QSplitter>
#include <QFont>
#include <QFontDatabase>
#include <QDateTime>
#include <QToolButton>
#include <QStyle>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <algorithm>

bool noteDateDescending(const MainWindow::Note &a, const MainWindow::Note &b) {
    return a.date > b.date;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Загрузка шрифтов
    QFontDatabase::addApplicationFont(":/fonts/Roboto-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-Bold.ttf");

    // Основной виджет и layout
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);

    // Левая колонка — категории
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setSpacing(10);

    QLabel *categoriesLabel = new QLabel("CATEGORIES");
    categoriesLabel->setStyleSheet("font-weight: bold; color: #455A64; font-size: 14px;");

    categoryBox = new QComboBox(this);
    categoryBox->setMinimumHeight(35);

    addCategoryButton = new QPushButton("Add Category");
    deleteCategoryButton = new QPushButton("Delete Category");

    addCategoryButton->setIcon(QIcon(":/icons/add.png"));
    deleteCategoryButton->setIcon(QIcon(":/icons/delete.png"));

    leftLayout->addWidget(categoriesLabel);
    leftLayout->addWidget(categoryBox);
    leftLayout->addWidget(addCategoryButton);
    leftLayout->addWidget(deleteCategoryButton);
    leftLayout->addStretch();

    connect(addCategoryButton, &QPushButton::clicked, this, &MainWindow::addCategory);
    connect(deleteCategoryButton, &QPushButton::clicked, this, &MainWindow::deleteCategory);
    connect(categoryBox, &QComboBox::currentTextChanged, this, &MainWindow::changeCategory);

    // Центральная колонка — заметки
    QVBoxLayout *centerLayout = new QVBoxLayout;
    centerLayout->setSpacing(10);

    QLabel *notesLabel = new QLabel("NOTES");
    notesLabel->setStyleSheet("font-weight: bold; color: #455A64; font-size: 14px;");

    noteList = new QListWidget(this);
    noteList->setFrameShape(QFrame::NoFrame);
    noteList->setAlternatingRowColors(true);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    deleteNoteButton = new QPushButton("Delete");
    saveCSVButton = new QPushButton("Export");

    deleteNoteButton->setIcon(QIcon(":/icons/delete.png"));
    saveCSVButton->setIcon(QIcon(":/icons/save.png"));

    buttonLayout->addWidget(deleteNoteButton);
    buttonLayout->addWidget(saveCSVButton);
    buttonLayout->setSpacing(10);

    centerLayout->addWidget(notesLabel);
    centerLayout->addWidget(noteList);
    centerLayout->addLayout(buttonLayout);

    connect(noteList, &QListWidget::itemClicked, this, &MainWindow::loadNoteForEdit);
    connect(deleteNoteButton, &QPushButton::clicked, this, &MainWindow::deleteNote);
    connect(saveCSVButton, &QPushButton::clicked, this, &MainWindow::saveToCSV);

    // Правая колонка — редактор
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->setSpacing(15);

    QLabel *editorLabel = new QLabel("EDITOR");
    editorLabel->setStyleSheet("font-weight: bold; color: #455A64; font-size: 14px;");

    QFormLayout *formLayout = new QFormLayout;
    formLayout->setVerticalSpacing(15);
    formLayout->setHorizontalSpacing(10);

    noteTitle = new QLineEdit(this);
    noteTitle->setPlaceholderText("Note title...");
    noteTitle->setMinimumHeight(35);

    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setCalendarPopup(true);
    dateEdit->setMinimumHeight(35);

    noteText = new QPlainTextEdit(this);
    noteText->setPlaceholderText("Write your thoughts here...");

    addNewNoteButton = new QPushButton("Save Note");
    addNewNoteButton->setMinimumHeight(40);

    formLayout->addRow("Title:", noteTitle);
    formLayout->addRow("Date:", dateEdit);
    formLayout->addRow("Text:", noteText);

    rightLayout->addWidget(editorLabel);
    rightLayout->addLayout(formLayout);
    rightLayout->addWidget(addNewNoteButton);
    rightLayout->addStretch();

    connect(addNewNoteButton, &QPushButton::clicked, this, &MainWindow::addNewNote);

    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(centerLayout, 2);
    mainLayout->addLayout(rightLayout, 3);

    setCentralWidget(central);
    setWindowTitle("Personal Journal");
    resize(900, 600);

    // Новый стиль
    QString styleSheet = R"(
        QWidget {
            background-color: #ECEFF1;
            font-family: 'Roboto';
            color: #37474F;
        }

        QComboBox, QLineEdit, QDateEdit, QPlainTextEdit {
            background-color: white;
            border: 1px solid #B0BEC5;
            border-radius: 5px;
            padding: 5px;
            font-size: 14px;
        }

        QPushButton {
            background-color: #546E7A;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 8px 16px;
            font-size: 14px;
        }

        QPushButton:hover {
            background-color: #455A64;
        }

        QPushButton:pressed {
            background-color: #37474F;
        }

        QListWidget {
            background-color: white;
            border: 1px solid #B0BEC5;
            border-radius: 5px;
            font-size: 14px;
        }

        QListWidget::item {
            padding: 8px;
            border-bottom: 1px solid #CFD8DC;
        }

        QListWidget::item:hover {
            background-color: #CFD8DC;
        }

        QListWidget::item:selected {
            background-color: #546E7A;
            color: white;
        }

        QLabel {
            font-weight: bold;
            color: #455A64;
            font-size: 14px;
        }
    )";
    setStyleSheet(styleSheet);

    // Загрузка заметок
    loadFromJson();

    if (!categorizedNotes.contains("Общее")) {
        categoryBox->addItem("Общее");
        categorizedNotes["Общее"] = {};
    }
    changeCategory(categoryBox->currentText());

    QFont font("Roboto", 10);
    this->setFont(font);
}

MainWindow::~MainWindow() {}

void MainWindow::addNewNote()
{
    QString title = noteTitle->text();
    QString text = noteText->toPlainText();
    QDate date = dateEdit->date();
    QString currentCategory = categoryBox->currentText();

    if (title.isEmpty() || text.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please fill in both title and text.");
        return;
    }

    Note newNote = {title, date.toString("yyyy-MM-dd"), text};

    int selected = noteList->currentRow();
    if (selected >= 0 && selected < categorizedNotes[currentCategory].size()) {
        categorizedNotes[currentCategory][selected] = newNote;
    } else {
        categorizedNotes[currentCategory].append(newNote);
    }

    std::sort(categorizedNotes[currentCategory].begin(), categorizedNotes[currentCategory].end(), noteDateDescending);
    refreshNoteList();
    saveToJson();

    // Очистка полей после сохранения
    noteList->setCurrentRow(-1);
    noteTitle->clear();
    noteText->clear();
    dateEdit->setDate(QDate::currentDate());
}

void MainWindow::saveToCSV()
{
    int index = noteList->currentRow();
    QString currentCategory = categoryBox->currentText();

    if (index < 0 || index >= categorizedNotes[currentCategory].size()) {
        QMessageBox::warning(this, "Warning", "Please select a note to export.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Save Selected Note", "", "*.csv");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "Cannot open file for writing.");
        return;
    }

    QTextStream out(&file);
    out << "Category,Title,Date,Text\n";

    const Note &note = categorizedNotes[currentCategory][index];
    QString safeText = note.text;
    safeText.replace("\"", "\"\"");

    out << "\"" << currentCategory << "\","
        << "\"" << note.title << "\","
        << "\"" << note.date << "\","
        << "\"" << safeText << "\"\n";

    file.close();
    QMessageBox::information(this, "Saved", "Selected note saved to CSV.");
}

void MainWindow::loadNoteForEdit(QListWidgetItem *item)
{
    int index = noteList->row(item);
    QString currentCategory = categoryBox->currentText();
    if (index >= 0 && index < categorizedNotes[currentCategory].size()) {
        const Note &note = categorizedNotes[currentCategory][index];
        noteTitle->setText(note.title);
        noteText->setPlainText(note.text);
        dateEdit->setDate(QDate::fromString(note.date, "yyyy-MM-dd"));
    }
}

void MainWindow::deleteNote()
{
    int index = noteList->currentRow();
    QString currentCategory = categoryBox->currentText();

    if (index >= 0 && index < categorizedNotes[currentCategory].size()) {
        categorizedNotes[currentCategory].removeAt(index);
        refreshNoteList();
        saveToJson();
        noteTitle->clear();
        noteText->clear();
        dateEdit->setDate(QDate::currentDate());
    }
}

void MainWindow::addCategory()
{
    bool ok;
    QString category = QInputDialog::getText(this, "Add Category", "Category name:", QLineEdit::Normal, "", &ok);
    if (ok && !category.isEmpty() && !categorizedNotes.contains(category)) {
        categoryBox->addItem(category);
        categorizedNotes[category] = {};
        saveToJson();
    }
}

void MainWindow::deleteCategory()
{
    QString currentCategory = categoryBox->currentText();
    if (currentCategory == "Общее") {
        QMessageBox::warning(this, "Warning", "Cannot delete default category.");
        return;
    }

    if (QMessageBox::question(this, "Delete Category", "Delete category and all its notes?") == QMessageBox::Yes) {
        categorizedNotes.remove(currentCategory);
        categoryBox->removeItem(categoryBox->currentIndex());
        changeCategory(categoryBox->currentText());
        saveToJson();
    }
}

void MainWindow::changeCategory(const QString &category)
{
    if (!category.isEmpty()) {
        std::sort(categorizedNotes[category].begin(), categorizedNotes[category].end(), noteDateDescending);
        refreshNoteList();
    }
}

void MainWindow::refreshNoteList()
{
    noteList->clear();
    QString currentCategory = categoryBox->currentText();
    for (const Note &note : categorizedNotes[currentCategory]) {
        noteList->addItem(note.title);
    }
}

void MainWindow::saveToJson()
{
    QJsonObject root;
    for (auto it = categorizedNotes.begin(); it != categorizedNotes.end(); ++it) {
        QJsonArray notesArray;
        for (const Note &note : it.value()) {
            QJsonObject noteObject;
            noteObject["title"] = note.title;
            noteObject["date"] = note.date;
            noteObject["text"] = note.text;
            notesArray.append(noteObject);
        }
        root[it.key()] = notesArray;
    }

    QJsonDocument doc(root);
    QFile file("notes.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void MainWindow::loadFromJson()
{
    QFile file("notes.json");
    if (!file.exists()) return;

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isObject()) {
            QJsonObject root = doc.object();
            for (const QString &category : root.keys()) {
                QJsonArray notesArray = root[category].toArray();
                QVector<Note> notes;
                for (const QJsonValue &val : notesArray) {
                    QJsonObject obj = val.toObject();
                    notes.append({obj["title"].toString(), obj["date"].toString(), obj["text"].toString()});
                }
                categorizedNotes[category] = notes;
                if (categoryBox->findText(category) == -1) {
                    categoryBox->addItem(category);
                }
            }
        }
    }
}
