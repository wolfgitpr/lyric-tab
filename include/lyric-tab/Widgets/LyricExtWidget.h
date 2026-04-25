#ifndef LYRIC_TAB_WIDGETS_LYRIC_EXT_WIDGET_H
#define LYRIC_TAB_WIDGETS_LYRIC_EXT_WIDGET_H

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <lyric-tab/Controls/LyricWrapView.h>

#include <lyric-tab/LyricTabConfig.h>
#include <lyric-tab/LyricTabGlobal.h>
#include <lyric-tab/Widgets/WidgetTypes.h>

namespace FillLyric
{
    class LYRIC_TAB_EXPORT LyricExtWidget final : public QWidget {
        Q_OBJECT

    public:
        explicit LyricExtWidget(int *notesCount, const LyricTabConfig &config,
                                const std::vector<std::string> &priorityG2pIds,
                                QMap<std::string, std::string> langToG2pId, QWidget *parent = nullptr);
        ~LyricExtWidget() override;

        LyricWrapView *wrapView() const;
        double fontSize() const;
        void setFoldLeftText(const QString &text);

    Q_SIGNALS:
        void modifyOption() const;
        void foldLeftRequested();
        void insertTextRequested();

    public Q_SLOTS:
        void onNotesCountChanged(const int &count) const;

    private:
        int *m_notesCount = nullptr;
        QHBoxLayout *m_tableTopLayout;

        QHBoxLayout *m_mainLayout;
        QVBoxLayout *m_tableLayout;
        QHBoxLayout *m_tableCountLayout;

        LyricWrapView *m_wrapView;

        QLabel *m_noteCountLabel;

        Button *m_btnFoldLeft;
        Button *m_btnInsertText;

        std::vector<std::string> m_priorityG2pIds;
        QMap<std::string, std::string> m_langToG2pId;
    };

} // namespace FillLyric

#endif // LYRIC_TAB_WIDGETS_LYRIC_EXT_WIDGET_H
