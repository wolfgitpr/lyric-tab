#include <lyric-tab/LyricTab.h>

#include <lyric-tab/Controls/CellList.h>
#include <lyric-tab/Controls/LyricCell.h>

#include <QDebug>
#include <QFileDialog>
#include <language-manager/ILanguageManager.h>
#include "../Utils/SplitLyric.h"

#include <QMessageBox>
#include <QTranslator>
#include <utility>

namespace FillLyric
{
    LyricTab::LyricTab(const QList<LangNote> &langNotes, QStringList priorityG2pIds, const LyricTabConfig &config,
                       QWidget *parent, const QString &transfile) :
        QWidget(parent), m_priorityG2pIds(std::move(priorityG2pIds)) {

        for (const auto &langNote : langNotes) {
            auto *note = new LangNote(langNote.lyric);
            note->language = note->language;
            note->category = note->category;
            note->g2pId = langNote.g2pId;
            m_langNotes.append(note);
        }

        const QString locale = QLocale::system().name();
        auto *translator = new QTranslator(this);
        if (QFile::exists(transfile) && translator->load(transfile)) {
            qDebug() << "LyricTab: Loaded translation from file system:" << transfile;
        } else if (translator->load(QString(":/share/translations/lyric-tab_%1.qm").arg(locale))) {
            qDebug() << "LyricTab: Loaded translation from resources:"
                     << QString(":/share/translations/lyric-tab_%1.qm").arg(locale);
        } else {
            qWarning() << "LyricTab: Failed to load translation";
        }
        QCoreApplication::installTranslator(translator);

        const auto langMgr = LangMgr::ILanguageManager::instance();
        langMgr->correct(m_langNotes);

        // textWidget
        m_lyricBaseWidget = new LyricBaseWidget(config);

        // lyricExtWidget
        m_lyricExtWidget = new LyricExtWidget(&notesCount, config);

        // lyric layout
        m_lyricLayout = new QHBoxLayout();
        m_lyricLayout->setContentsMargins(0, 0, 0, 0);
        m_lyricLayout->addWidget(m_lyricBaseWidget, 1);
        m_lyricLayout->addWidget(m_lyricExtWidget, 2);

        // main layout
        m_mainLayout = new QVBoxLayout(this);
        m_mainLayout->setContentsMargins(0, 10, 0, 10);
        m_mainLayout->addLayout(m_lyricLayout);

        connect(m_lyricBaseWidget, &LyricBaseWidget::modifyOption, this, &LyricTab::modifyOption);
        connect(m_lyricExtWidget, &LyricExtWidget::modifyOption, this, &LyricTab::modifyOption);

        connect(m_lyricBaseWidget->btnReReadNote, &QAbstractButton::clicked, this, &LyricTab::setLangNotes);

        // phonicWidget signals
        connect(m_lyricExtWidget->m_btnInsertText, &QAbstractButton::clicked, this,
                &LyricTab::_on_btnInsertText_clicked);
        connect(m_lyricBaseWidget->m_btnToTable, &QAbstractButton::clicked, this, &LyricTab::_on_btnToTable_clicked);

        // fold right
        connect(m_lyricBaseWidget->btnLyricPrev, &QPushButton::clicked, this,
                [this]()
                {
                    m_lyricExtWidget->setVisible(!m_lyricExtWidget->isVisible());
                    m_lyricBaseWidget->btnLyricPrev->setText(m_lyricExtWidget->isVisible() ? tr("Fold Preview")
                                                                                           : tr("Lyric Prev"));
                    m_lyricBaseWidget->m_btnToTable->setVisible(m_lyricExtWidget->isVisible());

                    if (!m_lyricExtWidget->isVisible()) {
                        Q_EMIT this->shrinkWindowRight(m_lyricBaseWidget->width() + 20);
                    } else {
                        Q_EMIT this->expandWindowRight();
                    }
                    modifyOption();
                });

        // fold left
        connect(m_lyricExtWidget->btnFoldLeft, &QPushButton::clicked, this,
                [this]()
                {
                    m_lyricBaseWidget->setVisible(!m_lyricBaseWidget->isVisible());
                    m_lyricExtWidget->btnFoldLeft->setText(m_lyricBaseWidget->isVisible() ? tr("Fold Left")
                                                                                          : tr("Expand Left"));
                    modifyOption();
                });

        const bool baseVisible = config.lyricBaseVisible;
        const bool extVisible = config.lyricExtVisible;

        if (!baseVisible) {
            m_lyricBaseWidget->setVisible(baseVisible);
            m_lyricExtWidget->btnFoldLeft->setText(tr("Expand Left"));
        }

        if (!extVisible) {
            m_lyricExtWidget->setVisible(extVisible);
            m_lyricBaseWidget->m_btnToTable->setVisible(extVisible);
            m_lyricBaseWidget->btnLyricPrev->setText(tr("Lyric Prev"));
        } else {
            m_lyricBaseWidget->btnLyricPrev->setText(tr("Fold Preview"));
        }

        m_lyricBaseWidget->skipSlur->setChecked(config.baseSkipSlur);
        connect(m_lyricBaseWidget->skipSlur, &QCheckBox::stateChanged, this, &LyricTab::setLangNotes);
    }

    LyricTab::~LyricTab() = default;

    void LyricTab::setLangNotes(bool warn) {
        const bool skipSlurRes = m_lyricBaseWidget->skipSlur->isChecked();

        bool setLangNotes = false;
        if (warn) {
            const QMessageBox::StandardButton res =
                QMessageBox::question(nullptr, tr("Preview Lyric"), tr("Split the lyric into Preview window?"),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if (res == QMessageBox::Yes)
                setLangNotes = true;
        }

        if (!warn || setLangNotes) {
            QStringList lyrics;
            QList<LangNote> langNotes;
            for (const auto &langNote : m_langNotes) {
                if (skipSlurRes && (langNote->language == "slur" || langNote->lyric == "-"))
                    continue;
                langNotes.append(*langNote);
                lyrics.append(langNote->lyric);
            }
            notesCount = static_cast<int>(langNotes.size());
            m_lyricBaseWidget->m_textEdit->setPlainText(lyrics.join(" "));
            m_lyricExtWidget->m_wrapView->init({langNotes});
        } else {
            disconnect(m_lyricBaseWidget->skipSlur, &QCheckBox::stateChanged, this, &LyricTab::setLangNotes);
            m_lyricBaseWidget->skipSlur->setCheckState(!skipSlurRes ? Qt::Checked : Qt::Unchecked);
            connect(m_lyricBaseWidget->skipSlur, &QCheckBox::stateChanged, this, &LyricTab::setLangNotes);
            modifyOption();
        }
    }

    QList<QList<LangNote>> LyricTab::exportLangNotes() const {
        if (m_lyricExtWidget->isVisible()) {
            return this->modelExport();
        }
        auto langNotes = m_lyricBaseWidget->splitLyric(m_lyricBaseWidget->m_textEdit->toPlainText());

        QList<QList<LangNote>> result;
        const auto langMgr = LangMgr::ILanguageManager::instance();
        for (auto &notes : langNotes) {
            QList<LangNote *> inputNotes;
            QList<LangNote> lineRes;

            for (auto &note : notes) {
                inputNotes.append(&note);
            }
            langMgr->correct(inputNotes, m_priorityG2pIds);
            LangMgr::ILanguageManager::convert(inputNotes);
            for (const auto &note : inputNotes) {
                lineRes.append(*note);
            }
            result.append(lineRes);
        }
        return langNotes;
    }

    bool LyricTab::exportSkipSlur() const { return m_lyricBaseWidget->skipSlur->isChecked(); }

    bool LyricTab::exportLanguage() const { return m_lyricExtWidget->exportLanguage->isChecked(); }

    QList<QList<LangNote>> LyricTab::modelExport() const {
        const auto cellLists = m_lyricExtWidget->m_wrapView->cellLists();

        QList<QList<LangNote>> noteList;
        for (const auto &cellList : cellLists) {
            QList<LangNote> notes;
            for (const auto &cell : cellList->m_cells) {
                const auto note = cell->note();
                notes.append(*note);
            }
            noteList.append(notes);
        }
        return noteList;
    }

    void LyricTab::_on_btnInsertText_clicked() const {
        const QString text = "Halloween蝉声--陪かな伴着qwe行云流浪---\nka回-忆-开始132后安静遥望远方"
                             "\n荒草覆没的古井--枯塘\n匀-散asdaw一缕过往\n";
        m_lyricBaseWidget->m_textEdit->setPlainText(text);
        m_lyricExtWidget->m_wrapView->init(CleanLyric::splitAuto(text));
    }

    void LyricTab::_on_btnToTable_clicked() const {
        const auto splitType = static_cast<SplitType>(m_lyricBaseWidget->m_splitComboBox->currentIndex());

        const QString text = m_lyricBaseWidget->m_textEdit->toPlainText();

        QList<QList<LangNote>> splitRes;
        if (splitType == Auto) {
            splitRes = CleanLyric::splitAuto(text);
        } else if (splitType == ByChar) {
            splitRes = CleanLyric::splitByChar(text);
        } else if (splitType == Custom) {
            splitRes = CleanLyric::splitCustom(text, m_lyricBaseWidget->m_splitters->text().split(' '));
        }

        const QMessageBox::StandardButton res =
            QMessageBox::question(nullptr, tr("Preview Lyric"), tr("Split the lyric into Preview window?"),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        if (res == QMessageBox::Yes)
            m_lyricExtWidget->m_wrapView->init(splitRes);
    }

    void LyricTab::modifyOption() {
        Q_EMIT this->modifyOptionSignal(
            {m_lyricBaseWidget->isVisible(), m_lyricExtWidget->isVisible(),
             m_lyricBaseWidget->m_textEdit->font().pointSizeF(), m_lyricBaseWidget->skipSlur->isChecked(),
             m_lyricBaseWidget->m_splitComboBox->currentIndex(), m_lyricExtWidget->m_wrapView->font().pointSizeF(),
             m_lyricExtWidget->m_wrapView->autoWrap(), m_lyricExtWidget->exportLanguage->isChecked()});
    }
} // FillLyric