#ifndef LYRICBASEWIDGET_H
#define LYRICBASEWIDGET_H

#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include <language-manager/LangCommon.h>
#include <lyric-tab/LyricTabConfig.h>

#include <lyric-tab/Controls/PhonicTextEdit.h>
#include <lyric-tab/LyricTabGlobal.h>


namespace FillLyric
{
#ifndef USE_LITE_CONTROLS
    using Button = QPushButton;
    using ComboBox = QComboBox;
    using LineEdit = QLineEdit;
#endif
    enum SplitType { Auto, ByChar, Custom };

    class LYRIC_TAB_EXPORT LyricBaseWidget final : public QWidget {
        Q_OBJECT
        friend class LyricTab;

    public:
        explicit LyricBaseWidget(const LyricTabConfig &config, QStringList priorityG2pIds, QWidget *parent = nullptr);
        ~LyricBaseWidget() override;

        QList<QList<LangNote>> splitLyric(const QString &lyric) const;

    Q_SIGNALS:
        void modifyOption() const;

    public Q_SLOTS:
        void _on_btnImportLrc_clicked();
        void _on_textEditChanged() const;
        void _on_splitComboBox_currentIndexChanged(int index) const;

    private:
        QVBoxLayout *m_mainLayout;
        QHBoxLayout *m_textTopLayout;
        QHBoxLayout *m_textBottomLayout;

        QWidget *m_optWidget;
        QVBoxLayout *m_optLayout;
        QHBoxLayout *m_optLabelLayout;
        QHBoxLayout *m_splitLayout;
        QHBoxLayout *m_skipSlurLayout;

        QLabel *m_textCountLabel;
        PhonicTextEdit *m_textEdit;

        // textEditTop
        Button *btnImportLrc;
        Button *btnReReadNote;
        Button *btnLyricPrev;

        // CheckBox
        QCheckBox *skipSlur;

        QLabel *m_optLabel;
        QPushButton *m_optButton;

        QLabel *m_splitLabel;
        ComboBox *m_splitComboBox;
        LineEdit *m_splitters;

        Button *m_btnToTable;

        QStringList m_priorityG2pIds;
    };


} // namespace FillLyric

#endif // LYRICBASEWIDGET_H
