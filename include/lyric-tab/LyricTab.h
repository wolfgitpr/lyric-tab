#ifndef DS_EDITOR_LITE_LYRICWIDGET_H
#define DS_EDITOR_LITE_LYRICWIDGET_H

#include <lyric-tab/LangCommon.h>

#include <lyric-tab/LyricTabConfig.h>
#include <lyric-tab/LyricTabGlobal.h>

#include <lyric-tab/Widgets/LyricBaseWidget.h>
#include <lyric-tab/Widgets/LyricExtWidget.h>

namespace FillLyric
{
    class LYRIC_TAB_EXPORT LyricTab final : public QWidget {
        Q_OBJECT
        friend class LyricDialog;

    public:
        explicit LyricTab(const QList<LangNote> &langNotes, const QStringList &priorityG2pIds = {},
                          QMap<QString, QString> langToG2pId = {}, const LyricTabConfig &config = {},
                          QWidget *parent = nullptr, const QString &transFile = "");
        ~LyricTab() override;

        void setLangNotes(bool warn = true);

        QList<QList<LangNote>> exportLangNotes() const;
        QList<QList<LangNote>> modelExport() const;

        bool exportSkipSlur() const;

        LyricBaseWidget *m_lyricBaseWidget;
        LyricExtWidget *m_lyricExtWidget;

    Q_SIGNALS:
        void shrinkWindowRight(int newWidth);
        void expandWindowRight();
        void modifyOptionSignal(FillLyric::LyricTabConfig config);

    public Q_SLOTS:
        void _on_btnInsertText_clicked() const;
        void _on_btnToTable_clicked() const;

    private:
        void modifyOption();

        std::vector<std::string> m_priorityG2pIds;
        QMap<std::string, std::string> m_langToG2pId;

        QList<LangNote *> m_langNotes;

        // Variables
        int notesCount = 0;

        // Layout
        QVBoxLayout *m_mainLayout;
        QHBoxLayout *m_lyricLayout;
    };

} // namespace FillLyric

#endif // DS_EDITOR_LITE_LYRICWIDGET_H
