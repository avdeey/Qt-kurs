#ifndef EMOJIMENUBUILDER_H
#define EMOJIMENUBUILDER_H

#include <QMenu>
#include <QStringList>
#include <QToolButton>
#include <functional>

class EmojiMenuBuilder {
public:
    static QMenu* createEmojiMenu(QWidget *parent, const std::function<void(const QString &)> &onEmojiSelected);
};

#endif // EMOJIMENUBUILDER_H
