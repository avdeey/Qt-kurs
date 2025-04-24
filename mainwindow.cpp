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
#include <algorithm>

// Функция сортировки по дате
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
    categoriesLabel->setStyleSheet("font-weight: bold; color: #5D5D5D; font-size: 14px;");

    categoryBox = new QComboBox(this);
    categoryBox->setMinimumHeight(35);

    addCategoryButton = new QPushButton("Add Category");
    deleteCategoryButton = new QPushButton("Delete Category");

    // Установка иконок для кнопок
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


    QVBoxLayout *centerLayout = new QVBoxLayout;
    centerLayout->setSpacing(10);

    QLabel *notesLabel = new QLabel("NOTES");
    notesLabel->setStyleSheet("font-weight: bold; color: #5D5D5D; font-size: 14px;");

    noteList = new QListWidget(this);
    noteList->setFrameShape(QFrame::NoFrame);
    noteList->setAlternatingRowColors(true);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    deleteNoteButton = new QPushButton("Delete");
    saveCSVButton = new QPushButton("Export");

    // Установка иконок для кнопок
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

    // Правая колонка — поля редактирования
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->setSpacing(15);

    QLabel *editorLabel = new QLabel("EDITOR");
    editorLabel->setStyleSheet("font-weight: bold; color: #5D5D5D; font-size: 14px;");

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



    QString styleSheet = R"(
        QWidget {
            background-color: #F5F5F5;
            font-family: 'Roboto';
            color: black;
        }

        QComboBox, QLineEdit, QDateEdit, QPlainTextEdit {
            background-color: white;
            border: 1px solid #D3D3D3;
            border-radius: 4px;
            padding: 5px;
            font-size: 14px;
            color: black;
        }

        QPlainTextEdit {
            min-height: 200px;
        }

        QPushButton {
            background-color: #FF69B4;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 15px;
            font-size: 14px;
        }

        QPushButton:hover {
            background-color: #E55A9D;
        }

        QPushButton:pressed {
            background-color: #CC4B86;
        }

        QListWidget {
            background-color: white;
            border: 1px solid #D3D3D3;
            border-radius: 4px;
            font-size: 14px;
        }

        QListWidget::item {
            padding: 8px;
            border-bottom: 1px solid #F0F0F0;
        }

        QListWidget::item:hover {
            background-color: #FFE6F0;
        }

        QListWidget::item:selected {
            background-color: #FF69B4;
            color: white;
        }

        QLabel {
            color: #5D5D5D;
            font-size: 14px;
        }
    )";

    setStyleSheet(styleSheet);


    categoryBox->addItem("Общее");
    categorizedNotes["Общее"] = {};
    changeCategory("Общее");

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
