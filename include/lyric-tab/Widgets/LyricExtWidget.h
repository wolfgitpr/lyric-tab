#ifndef LYRICEXTWIDGET_H
#define LYRICEXTWIDGET_H

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <lyric-tab/Controls/LyricWrapView.h>

#include <lyric-tab/LyricTabConfig.h>
#include <lyric-tab/LyricTabGlobal.h>

namespace FillLyric
{
#ifndef USE_LITE_CONTROLS
    using Button = QPushButton;
    using SwitchButton = QCheckBox;
#endif
    class LYRIC_TAB_EXPORT LyricExtWidget final : public QWidget {
        Q_OBJECT
        friend class LyricTab;

    public:
        explicit LyricExtWidget(int *notesCount, LyricTabConfig config, QStringList priorityG2pIds,
                                QWidget *parent = nullptr);
        ~LyricExtWidget() override;

    Q_SIGNALS:
        void modifyOption() const;

    public Q_SLOTS:
        // count
        void _on_notesCountChanged(const int &count) const;

    private:
        int *notesCount = nullptr;
        QHBoxLayout *m_tableTopLayout;

        QHBoxLayout *m_mainLayout;
        QVBoxLayout *m_tableLayout;
        QHBoxLayout *m_tableCountLayout;

        // Widgets
        LyricWrapView *m_wrapView;

        // Labels
        QLabel *noteCountLabel;

        Button *btnFoldLeft;
        Button *m_btnInsertText;

        QStringList m_priorityG2pIds;
    };

} // namespace FillLyric

#endif // LYRICEXTWIDGET_H
