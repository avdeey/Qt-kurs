/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftLayout;
    QComboBox *categoryBox;
    QPushButton *addCategoryButton;
    QPushButton *deleteCategoryButton;
    QPushButton *renameCategoryButton;
    QListView *noteListView;
    QLineEdit *lineEdit;
    QHBoxLayout *actionButtonsLayout;
    QPushButton *pushButton;
    QPushButton *deleteNoteButton;
    QPushButton *recentlyDeletedButton;
    QToolButton *sortButton;
    QVBoxLayout *rightLayout;
    QFormLayout *formLayout;
    QLabel *labelTitle;
    QLineEdit *noteTitle;
    QLabel *labelDate;
    QTextEdit *noteText;
    QHBoxLayout *buttonsLayout;
    QToolButton *emojiButton;
    QToolButton *insertImageButton;
    QToolButton *colorButton;
    QToolButton *boldButton;
    QToolButton *italicButton;
    QDateEdit *dateEdit;
    QPushButton *saveNoteButton;
    QPushButton *resetColorButton;
    QPushButton *exportOdtButton;
    QHBoxLayout *horizontalLayout_3;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(900, 613);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("\n"
"QWidget {\n"
" background-color: white;\n"
" color: #333333;\n"
" font-family: 'Segoe UI', 'Arial';\n"
" font-size: 14px;\n"
"}\n"
"\n"
"QLineEdit, QPlainTextEdit, QDateEdit, QComboBox {\n"
" background-color: white;\n"
" border: 1px solid #cccccc;\n"
" border-radius: 6px;\n"
" padding: 6px;\n"
" color: #333333;\n"
"}\n"
"\n"
"QPushButton, QToolButton {\n"
" background: transparent;\n"
" border: none;\n"
" padding: 5px;\n"
" margin: 1px;\n"
"}\n"
"\n"
"QPushButton:hover, QToolButton:hover {\n"
" background: rgba(240, 240, 240, 50);\n"
"}\n"
"\n"
"QPushButton:pressed, QToolButton:pressed {\n"
" background: rgba(224, 224, 224, 70);\n"
"}\n"
"\n"
"QToolButton {\n"
" border: 1px solid #ccc;\n"
" border-radius: 4px;\n"
"}\n"
"\n"
"QToolButton:hover {\n"
" border-color: #aaa;\n"
"}\n"
"\n"
"QToolButton#emojiButton {\n"
" border: none !important;\n"
" margin: 0;\n"
" padding: 0;\n"
"}\n"
"\n"
"QToolButton#sortButton {\n"
" border: none;\n"
"}\n"
"\n"
"QListWidget {\n"
" background-color: white;\n"
" border: 1px "
                        "solid #cccccc;\n"
" border-radius: 6px;\n"
"}\n"
"\n"
"QListWidget::item:selected {\n"
" background-color: #4a90e2;\n"
" color: #ffffff;\n"
"}\n"
"\n"
"QComboBox {\n"
" background-color: white;\n"
" padding: 6px;\n"
" color: #333333;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
" background-color: white;\n"
" selection-background-color: #4a90e2;\n"
" selection-color: #ffffff;\n"
"}\n"
"\n"
"QToolButton#insertImageButton {\n"
" background: transparent;\n"
"}\n"
"\n"
"QToolButton#colorButton {\n"
"}\n"
"\n"
"QToolButton#emojiButton::menu-indicator {\n"
" image: none;\n"
"}\n"
"    "));
        mainLayout = new QHBoxLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        leftLayout = new QVBoxLayout();
        leftLayout->setObjectName("leftLayout");
        categoryBox = new QComboBox(centralwidget);
        categoryBox->setObjectName("categoryBox");

        leftLayout->addWidget(categoryBox);

        addCategoryButton = new QPushButton(centralwidget);
        addCategoryButton->setObjectName("addCategoryButton");

        leftLayout->addWidget(addCategoryButton);

        deleteCategoryButton = new QPushButton(centralwidget);
        deleteCategoryButton->setObjectName("deleteCategoryButton");

        leftLayout->addWidget(deleteCategoryButton);

        renameCategoryButton = new QPushButton(centralwidget);
        renameCategoryButton->setObjectName("renameCategoryButton");

        leftLayout->addWidget(renameCategoryButton);

        noteListView = new QListView(centralwidget);
        noteListView->setObjectName("noteListView");

        leftLayout->addWidget(noteListView);

        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName("lineEdit");

        leftLayout->addWidget(lineEdit);

        actionButtonsLayout = new QHBoxLayout();
        actionButtonsLayout->setObjectName("actionButtonsLayout");
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");

        actionButtonsLayout->addWidget(pushButton);

        deleteNoteButton = new QPushButton(centralwidget);
        deleteNoteButton->setObjectName("deleteNoteButton");

        actionButtonsLayout->addWidget(deleteNoteButton);

        recentlyDeletedButton = new QPushButton(centralwidget);
        recentlyDeletedButton->setObjectName("recentlyDeletedButton");

        actionButtonsLayout->addWidget(recentlyDeletedButton);

        sortButton = new QToolButton(centralwidget);
        sortButton->setObjectName("sortButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/sort-up.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon.addFile(QString::fromUtf8(":/icons/sort-down.png"), QSize(), QIcon::Mode::Selected, QIcon::State::Off);
        sortButton->setIcon(icon);

        actionButtonsLayout->addWidget(sortButton);


        leftLayout->addLayout(actionButtonsLayout);


        mainLayout->addLayout(leftLayout);

        rightLayout = new QVBoxLayout();
        rightLayout->setObjectName("rightLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        labelTitle = new QLabel(centralwidget);
        labelTitle->setObjectName("labelTitle");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelTitle);

        noteTitle = new QLineEdit(centralwidget);
        noteTitle->setObjectName("noteTitle");

        formLayout->setWidget(0, QFormLayout::FieldRole, noteTitle);

        labelDate = new QLabel(centralwidget);
        labelDate->setObjectName("labelDate");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelDate);

        noteText = new QTextEdit(centralwidget);
        noteText->setObjectName("noteText");

        formLayout->setWidget(2, QFormLayout::SpanningRole, noteText);

        buttonsLayout = new QHBoxLayout();
        buttonsLayout->setObjectName("buttonsLayout");
        emojiButton = new QToolButton(centralwidget);
        emojiButton->setObjectName("emojiButton");

        buttonsLayout->addWidget(emojiButton);

        insertImageButton = new QToolButton(centralwidget);
        insertImageButton->setObjectName("insertImageButton");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/fajly_uwww2eho71zx_512.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        insertImageButton->setIcon(icon1);

        buttonsLayout->addWidget(insertImageButton);

        colorButton = new QToolButton(centralwidget);
        colorButton->setObjectName("colorButton");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/tsvet_1k4om45p2hvi_512.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        colorButton->setIcon(icon2);

        buttonsLayout->addWidget(colorButton);

        boldButton = new QToolButton(centralwidget);
        boldButton->setObjectName("boldButton");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/b_ia6b40coq1vo_512.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        boldButton->setIcon(icon3);

        buttonsLayout->addWidget(boldButton);

        italicButton = new QToolButton(centralwidget);
        italicButton->setObjectName("italicButton");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/letter_i_4pqtn0a404ow_512.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        italicButton->setIcon(icon4);

        buttonsLayout->addWidget(italicButton);


        formLayout->setLayout(3, QFormLayout::SpanningRole, buttonsLayout);

        dateEdit = new QDateEdit(centralwidget);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setCalendarPopup(true);

        formLayout->setWidget(1, QFormLayout::FieldRole, dateEdit);


        rightLayout->addLayout(formLayout);

        saveNoteButton = new QPushButton(centralwidget);
        saveNoteButton->setObjectName("saveNoteButton");

        rightLayout->addWidget(saveNoteButton);

        resetColorButton = new QPushButton(centralwidget);
        resetColorButton->setObjectName("resetColorButton");

        rightLayout->addWidget(resetColorButton);

        exportOdtButton = new QPushButton(centralwidget);
        exportOdtButton->setObjectName("exportOdtButton");

        rightLayout->addWidget(exportOdtButton);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");

        rightLayout->addLayout(horizontalLayout_3);


        mainLayout->addLayout(rightLayout);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Diary App", nullptr));
        addCategoryButton->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\272\320\260\321\202\320\265\320\263\320\276\321\200\320\270\321\216", nullptr));
        deleteCategoryButton->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\272\320\260\321\202\320\265\320\263\320\276\321\200\320\270\321\216", nullptr));
        renameCategoryButton->setText(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\320\275\320\270\321\202\321\214 \320\272\320\260\321\202\320\265\320\263\320\276\321\200\320\270\321\216", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\270\321\201\320\272 \320\267\320\260\320\274\320\265\321\202\320\276\320\272...", nullptr));
#if QT_CONFIG(tooltip)
        pushButton->setToolTip(QCoreApplication::translate("MainWindow", "\320\241\320\261\321\200\320\276\321\201\320\270\321\202\321\214 \320\262\321\213\320\264\320\265\320\273\320\265\320\275\320\270\320\265 \320\267\320\260\320\274\320\265\321\202\320\272\320\270", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\274\320\265\320\275\320\270\321\202\321\214 \320\262\321\213\320\261\320\276\321\200", nullptr));
        deleteNoteButton->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        recentlyDeletedButton->setText(QCoreApplication::translate("MainWindow", "\320\235\320\265\320\264\320\260\320\262\320\275\320\276 \321\203\320\264\320\260\320\273\320\265\320\275\320\275\321\213\320\265", nullptr));
        labelTitle->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265:", nullptr));
        labelDate->setText(QCoreApplication::translate("MainWindow", "\320\224\320\260\321\202\320\260:", nullptr));
        boldButton->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        italicButton->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        saveNoteButton->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        resetColorButton->setText(QCoreApplication::translate("MainWindow", "\320\241\320\261\321\200\320\276\321\201\320\270\321\202\321\214 \321\206\320\262\320\265\321\202", nullptr));
        exportOdtButton->setText(QCoreApplication::translate("MainWindow", "\320\255\320\272\321\201\320\277\320\276\321\200\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
