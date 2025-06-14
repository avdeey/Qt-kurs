#include "NoteExporter.h"
#include <QTextDocumentWriter>

bool NoteExporter::exportToOdt(const QTextDocument *doc, const QString &filePath) {
    QTextDocumentWriter writer(filePath, "odf");
    return writer.write(doc);
}
