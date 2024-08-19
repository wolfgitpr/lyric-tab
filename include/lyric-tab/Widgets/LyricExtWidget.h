#ifndef LYRICEXTWIDGET_H
#define LYRICEXTWIDGET_H

#include <QWidget>

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
    using LineEdit = QLineEdit;
#endif
    class LYRIC_TAB_EXPORT LyricExtWidget final : public QWidget {
        Q_OBJECT
        friend class LyricTab;

    public:
        explicit LyricExtWidget(int *notesCount, LyricTabConfig config, QWidget *parent = nullptr);
        ~LyricExtWidget() override;

    Q_SIGNALS:
        void modifyOption() const;

    public Q_SLOTS:
        // count
        void _on_notesCountChanged(const int &count) const;

    private:
        QUndoStack *m_history;

        int *notesCount = nullptr;
        QHBoxLayout *m_tableTopLayout;

        QHBoxLayout *m_mainLayout;
        QVBoxLayout *m_tableLayout;
        QHBoxLayout *m_tableCountLayout;
        QHBoxLayout *m_epOptLabelLayout;

        QWidget *m_epOptWidget;
        QVBoxLayout *m_epOptLayout;

        // Widgets
        LyricWrapView *m_wrapView;

        // Labels
        QLabel *noteCountLabel;

        Button *btnFoldLeft;
        QLabel *autoWrapLabel;
        SwitchButton *autoWrap;
        QPushButton *btnUndo;
        QPushButton *btnRedo;
        Button *m_btnInsertText;

        QLabel *exportOptLabel;
        QPushButton *exportOptButton;

        QCheckBox *exportLanguage;
    };

} // namespace FillLyric

#endif // LYRICEXTWIDGET_H
