#include <lyric-tab/LyricTab.h>

#include <lyric-tab/Controls/CellList.h>
#include <lyric-tab/Controls/LyricCell.h>

#include <LangCore/Core/Manager.h>
#include <QFileDialog>
#include "../Utils/SplitLyric.h"

#include <QMessageBox>
#include <QTranslator>
#include <utility>

namespace FillLyric
{
    LyricTab::LyricTab(const QList<LangNote> &langNotes, QStringList priorityG2pIds, const LyricTabConfig &config,
                       QWidget *parent, const QString &transFile) :
        QWidget(parent), m_priorityG2pIds(std::move(priorityG2pIds)) {

        for (const auto &langNote : langNotes)
            m_langNotes.append(new LangNote(langNote.lyric));

        const QString locale = QLocale::system().name();
        auto *translator = new QTranslator(this);
        if (QFile::exists(transFile) && translator->load(transFile)) {
            qDebug() << "LyricTab: Loaded translation from file system:" << transFile;
        } else if (translator->load(QString(":/share/translations/lyric-tab_%1.qm").arg(locale))) {
            qDebug() << "LyricTab: Loaded translation from resources:"
                     << QString(":/share/translations/lyric-tab_%1.qm").arg(locale);
        } else {
            qWarning() << "LyricTab: Failed to load translation";
        }
        QCoreApplication::installTranslator(translator);

        const auto langMgr = LangCore::Manager::instance();
        std::vector<std::string> taggerInput;
        for (const auto &note : m_langNotes)
            taggerInput.push_back(note->lyric.toStdString());
        const auto splitRes = langMgr->tag(taggerInput, false, {});

        std::vector<LangCore::G2pInput *> g2pInputs;
        for (const auto &taggerRes : splitRes)
            g2pInputs.push_back(new LangCore::G2pInput(taggerRes.lyric, taggerRes.language));

        const auto g2pRes = langMgr->convert(g2pInputs);
        for (int i = 0; i < g2pRes.size(); i++) {
            m_langNotes[i]->language = splitRes[i].language.c_str();
            m_langNotes[i]->g2pId = splitRes[i].language.c_str();
            m_langNotes[i]->syllable = g2pRes[i].pronunciation.c_str();
            m_langNotes[i]->candidates = QStringList({g2pRes[i].pronunciation.begin(), g2pRes[i].pronunciation.end()});
        }

        // textWidget
        m_lyricBaseWidget = new LyricBaseWidget(config, m_priorityG2pIds);

        // lyricExtWidget
        m_lyricExtWidget = new LyricExtWidget(&notesCount, config, m_priorityG2pIds);

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
                [this]
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
                [this]
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
        connect(m_lyricBaseWidget->skipSlur, &QCheckBox::checkStateChanged, this, &LyricTab::setLangNotes);
    }

    LyricTab::~LyricTab() = default;

    void LyricTab::setLangNotes(const bool warn) {
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
                if (skipSlurRes && (langNote->g2pId == "slur" || langNote->lyric == "-"))
                    continue;
                langNotes.append(*langNote);
                lyrics.append(langNote->lyric);
            }
            notesCount = static_cast<int>(langNotes.size());
            m_lyricBaseWidget->m_textEdit->setPlainText(lyrics.join(" "));
            m_lyricExtWidget->m_wrapView->init({langNotes});
        } else {
            disconnect(m_lyricBaseWidget->skipSlur, &QCheckBox::checkStateChanged, this, &LyricTab::setLangNotes);
            m_lyricBaseWidget->skipSlur->setCheckState(!skipSlurRes ? Qt::Checked : Qt::Unchecked);
            connect(m_lyricBaseWidget->skipSlur, &QCheckBox::checkStateChanged, this, &LyricTab::setLangNotes);
            modifyOption();
        }
    }

    QList<QList<LangNote>> LyricTab::exportLangNotes() const {
        if (m_lyricExtWidget->isVisible()) {
            return this->modelExport();
        }
        auto langNotes = m_lyricBaseWidget->splitLyric(m_lyricBaseWidget->m_textEdit->toPlainText());

        QList<QList<LangNote>> result;
        const auto langMgr = LangCore::Manager::instance();
        for (auto &notes : langNotes) {
            QList<LangNote *> inputNotes;
            QList<LangNote> lineRes;

            for (auto &note : notes)
                inputNotes.append(&note);

            std::vector<std::string> taggerInput;
            for (const auto &note : inputNotes)
                taggerInput.push_back(note->lyric.toStdString());
            const auto splitRes = langMgr->tag(taggerInput, false, {});

            std::vector<LangCore::G2pInput *> g2pInputs;
            for (const auto &note : splitRes)
                g2pInputs.push_back(new LangCore::G2pInput(note.lyric, note.language));

            const auto g2pRes = langMgr->convert(g2pInputs);
            for (int i = 0; i < g2pRes.size(); i++) {
                inputNotes[i]->language = splitRes[i].language.c_str();
                inputNotes[i]->syllable = g2pRes[i].pronunciation.c_str();
                inputNotes[i]->candidates = {g2pRes[i].pronunciation.begin(), g2pRes[i].pronunciation.end()};
            }

            for (const auto &note : inputNotes) {
                lineRes.append(*note);
            }
            result.append(lineRes);
        }
        return langNotes;
    }

    bool LyricTab::exportSkipSlur() const { return m_lyricBaseWidget->skipSlur->isChecked(); }

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
        const QString text = "halloween蝉 声--陪かな伴着qwe行云流浪---\nka回-忆-开始132后安静遥望远方"
                             "\n荒草覆没的古井--枯塘\n匀-散asdaw一缕过往\n";
        m_lyricBaseWidget->m_textEdit->setPlainText(text);
        m_lyricExtWidget->m_wrapView->init(CleanLyric::splitAuto(text, m_priorityG2pIds));
    }

    void LyricTab::_on_btnToTable_clicked() const {
        const auto splitType = static_cast<SplitType>(m_lyricBaseWidget->m_splitComboBox->currentIndex());

        const QString text = m_lyricBaseWidget->m_textEdit->toPlainText();

        QList<QList<LangNote>> splitRes;
        if (splitType == Auto) {
            splitRes = CleanLyric::splitAuto(text, m_priorityG2pIds);
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
             m_lyricBaseWidget->m_splitComboBox->currentIndex(), m_lyricExtWidget->m_wrapView->font().pointSizeF()});
    }
} // namespace FillLyric
