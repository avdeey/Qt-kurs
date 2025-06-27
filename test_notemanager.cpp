#include <QtTest>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QListView>
#include <QStandardItemModel>
#include "MainWindow.h"

class TestMainWindowUI : public QObject {
    Q_OBJECT

private:
    MainWindow *window;

private slots:
    void initTestCase();
    void test_addNoteUI();
    void cleanupTestCase();
};

void TestMainWindowUI::initTestCase() {
    window = new MainWindow;
    window->show();
    QVERIFY(QTest::qWaitForWindowActive(window));  // Убедимся, что окно активировано
}

void TestMainWindowUI::cleanupTestCase() {
    delete window;
}

void TestMainWindowUI::test_addNoteUI() {
    auto *titleEdit = window->findChild<QLineEdit*>("noteTitle");
    auto *textEdit = window->findChild<QTextEdit*>("noteText");
    auto *dateEdit = window->findChild<QDateEdit*>("dateEdit");
    auto *addButton = window->findChild<QPushButton*>("saveNoteButton");
    auto *listView = window->findChild<QListView*>("noteListView");

    QVERIFY(titleEdit);
    QVERIFY(textEdit);
    QVERIFY(dateEdit);
    QVERIFY(addButton);
    QVERIFY(listView);

    // Задание значений
    titleEdit->setText("UI-Тестовая заметка");
    textEdit->setPlainText("Текст из UI-теста");
    dateEdit->setDate(QDate::currentDate());

    QTest::qWait(100);  // Немного подождать для UI
    QTest::mouseClick(addButton, Qt::LeftButton);

    // Проверка
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(listView->model());
    QVERIFY(model);
    bool found = false;
    for (int i = 0; i < model->rowCount(); ++i) {
        if (model->item(i)->text() == "UI-Тестовая заметка") {
            found = true;
            break;
        }
    }

    QVERIFY(found);
}


QTEST_MAIN(TestMainWindowUI)

#include "test_notemanager.moc"
