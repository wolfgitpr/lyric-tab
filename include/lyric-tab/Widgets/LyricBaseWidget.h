#ifndef LYRIC_TAB_WIDGETS_LYRIC_BASE_WIDGET_H
#define LYRIC_TAB_WIDGETS_LYRIC_BASE_WIDGET_H

#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include <lyric-tab/Controls/PhonicTextEdit.h>
#include <lyric-tab/LangCommon.h>
#include <lyric-tab/LyricTabConfig.h>
#include <lyric-tab/LyricTabGlobal.h>
#include <lyric-tab/Widgets/WidgetTypes.h>


namespace FillLyric
{
    enum SplitType { Auto, ByChar, Custom };

    class LYRIC_TAB_EXPORT LyricBaseWidget final : public QWidget {
        Q_OBJECT

    public:
        explicit LyricBaseWidget(const LyricTabConfig &config, std::vector<std::string> priorityG2pIds,
                                 QMap<std::string, std::string> langToG2pId, QWidget *parent = nullptr);
        ~LyricBaseWidget() override;

        QString lyricText() const;
        void setLyricText(const QString &text);
        bool skipSlur() const;
        void setSkipSlur(bool skip);
        int splitMode() const;
        QString splitters() const;
        double fontSize() const;
        void setToTableVisible(bool visible);
        void setLyricPrevText(const QString &text);

        QList<QList<LangNote>> splitLyric(const QString &lyric) const;

    Q_SIGNALS:
        void modifyOption() const;
        void reReadNoteRequested();
        void toTableRequested();
        void lyricPrevRequested();

    public Q_SLOTS:
        void onBtnImportLrcClicked();
        void onTextEditChanged() const;
        void onSplitComboBoxCurrentIndexChanged(int index) const;

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

        Button *m_btnImportLrc;
        Button *m_btnReReadNote;
        Button *m_btnLyricPrev;

        QCheckBox *m_skipSlur;

        QLabel *m_optLabel;
        QPushButton *m_optButton;

        QLabel *m_splitLabel;
        ComboBox *m_splitComboBox;
        LineEdit *m_splitters;

        Button *m_btnToTable;

        std::vector<std::string> m_priorityG2pIds;
        QMap<std::string, std::string> m_langToG2pId;
    };


} // namespace FillLyric

#endif // LYRIC_TAB_WIDGETS_LYRIC_BASE_WIDGET_H
