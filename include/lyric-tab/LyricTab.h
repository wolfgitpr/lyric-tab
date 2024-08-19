#ifndef DS_EDITOR_LITE_LYRICWIDGET_H
#define DS_EDITOR_LITE_LYRICWIDGET_H

#include <QVBoxLayout>

#include <language-manager/LangCommon.h>

#include <lyric-tab/LyricTabConfig.h>
#include <lyric-tab/LyricTabGlobal.h>

#include <lyric-tab/Widgets/LyricBaseWidget.h>
#include <lyric-tab/Widgets/LyricExtWidget.h>

namespace FillLyric
{
    class LYRIC_TAB_EXPORT LyricTab : public QWidget {
        Q_OBJECT

        friend class LyricDialog;

    public:
        explicit LyricTab(QList<LangNote> langNotes, LyricTabConfig config = {}, QWidget *parent = nullptr);
        ~LyricTab() override;

        void setLangNotes();

        [[nodiscard]] QList<QList<LangNote>> exportLangNotes() const;
        [[nodiscard]] QList<QList<LangNote>> modelExport() const;

        [[nodiscard]] bool exportSkipSlur() const;
        [[nodiscard]] bool exportLanguage() const;

        LyricBaseWidget *m_lyricBaseWidget;
        LyricExtWidget *m_lyricExtWidget;


    Q_SIGNALS:
        void shrinkWindowRight(int newWidth);
        void expandWindowRight();
        void modifyOptionSignal(LyricTabConfig config);

    public Q_SLOTS:
        void _on_btnInsertText_clicked() const;
        void _on_btnToTable_clicked() const;

    private:
        void modifyOption();

        QList<LangNote> m_langNotes;

        // Variables
        int notesCount = 0;

        // Layout
        QVBoxLayout *m_mainLayout;
        QHBoxLayout *m_lyricLayout;
    };

} // namespace FillLyric

#endif // DS_EDITOR_LITE_LYRICWIDGET_H
