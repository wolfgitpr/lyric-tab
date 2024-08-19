#ifndef DS_EDITOR_LITE_PHONICTEXTEDIT_H
#define DS_EDITOR_LITE_PHONICTEXTEDIT_H

#include <QObject>
#include <QPlainTextEdit>

#include <lyric-tab/LyricTabGlobal.h>

namespace FillLyric {
    class LYRIC_TAB_EXPORT PhonicTextEdit final : public QPlainTextEdit {
        Q_OBJECT
    public:
        explicit PhonicTextEdit(QWidget *parent = nullptr);

    Q_SIGNALS:
        void fontChanged();

    protected:
        void wheelEvent(QWheelEvent *event) override;
        void contextMenuEvent(QContextMenuEvent *event) override;
    };
}

#endif // DS_EDITOR_LITE_PHONICTEXTEDIT_H
