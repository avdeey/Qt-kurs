#include "EmojiMenuBuilder.h"
#include <QAction>

QMenu* EmojiMenuBuilder::createEmojiMenu(QWidget *parent, const std::function<void(const QString &)> &onEmojiSelected) {
    QMenu *emojiMenu = new QMenu(parent);
    QStringList emojis = {"😀", "😢", "🙂", "🤔", "😊", "👍", "❤️"};

    for (const QString &emoji : emojis) {
        QAction *action = emojiMenu->addAction(emoji);
        QObject::connect(action, &QAction::triggered, [emoji, onEmojiSelected]() {
            onEmojiSelected(emoji);
        });
    }
    return emojiMenu;
}
