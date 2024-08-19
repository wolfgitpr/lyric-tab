#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QKeyEvent>
#include <QDialog>

#include <lyric-tab/LyricTabGlobal.h>

namespace FillLyric {
    class LYRIC_TAB_EXPORT EditDialog final : public QDialog {
    public:
        explicit EditDialog(const QString &lyric, const QRectF &rect, const QFont &font,
                            QWidget *parent = nullptr);

        QString text;

    protected:
        bool event(QEvent *event) override;
    };
} // FillLyric

#endif // EDITDIALOG_H
