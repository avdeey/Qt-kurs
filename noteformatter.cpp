#include "NoteFormatter.h"
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextImageFormat>
#include <QFileInfo>
#include <QUuid>
#include <QDir>

void NoteFormatter::applyTextColor(QTextEdit *editor, const QColor &color) {
    QTextCursor cursor = editor->textCursor();

    QTextCharFormat format;
    format.setForeground(color);

    if (cursor.hasSelection()) {
        cursor.mergeCharFormat(format);
    }

    editor->mergeCurrentCharFormat(format);
}

void NoteFormatter::resetTextColor(QTextEdit *editor) {
    QTextCursor cursor(editor->document());
    cursor.select(QTextCursor::Document);
    QTextCharFormat format;
    format.setForeground(Qt::black);
    cursor.mergeCharFormat(format);
}

void NoteFormatter::insertImage(QTextEdit *editor, const QString &imagePath) {
    QDir().mkpath("notes_images");
    QString newName = "notes_images/" + QUuid::createUuid().toString() + "." + QFileInfo(imagePath).suffix();
    if (QFile::copy(imagePath, newName)) {
        QTextCursor cursor = editor->textCursor();
        QTextImageFormat imageFormat;
        imageFormat.setName(newName);
        imageFormat.setWidth(200);
        cursor.insertImage(imageFormat);
    }
}

void NoteFormatter::insertEmoji(QTextEdit *editor, const QString &emoji) {
    editor->insertPlainText(emoji);
}

QColor NoteFormatter::getCurrentTextColor(QTextEdit *editor) {
    QTextCursor cursor(editor->document());
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
    QColor color = cursor.charFormat().foreground().color();
    return color.isValid() ? color : Qt::black;
}
void NoteFormatter::toggleBold(QTextEdit *editor) {
    QTextCharFormat format;
    QFont::Weight currentWeight = static_cast<QFont::Weight>(editor->fontWeight());
    format.setFontWeight(currentWeight == QFont::Bold ? QFont::Normal : QFont::Bold);
    editor->textCursor().mergeCharFormat(format);
    editor->mergeCurrentCharFormat(format);
}

void NoteFormatter::toggleItalic(QTextEdit *editor) {
    QTextCharFormat format;
    bool italic = editor->fontItalic();
    format.setFontItalic(!italic);
    editor->textCursor().mergeCharFormat(format);
    editor->mergeCurrentCharFormat(format);
}

