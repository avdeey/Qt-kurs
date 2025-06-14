#ifndef NOTEEXPORTER_H
#define NOTEEXPORTER_H

#include <QString>
#include <QTextDocument>

class NoteExporter {
public:
    static bool exportToOdt(const QTextDocument *doc, const QString &filePath);
};

#endif // NOTEEXPORTER_H
