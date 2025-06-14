#ifndef NOTEFORMATTER_H
#define NOTEFORMATTER_H

#include <QTextEdit>
#include <QColor>

class NoteFormatter {
public:
    static void applyTextColor(QTextEdit *editor, const QColor &color);
    static void resetTextColor(QTextEdit *editor);
    static void insertImage(QTextEdit *editor, const QString &imagePath);
    static void insertEmoji(QTextEdit *editor, const QString &emoji);
    static QColor getCurrentTextColor(QTextEdit *editor);

    static void toggleBold(QTextEdit *editor);    // Новое: жирный
    static void toggleItalic(QTextEdit *editor);  // Новое: курсив
};

#endif // NOTEFORMATTER_H
