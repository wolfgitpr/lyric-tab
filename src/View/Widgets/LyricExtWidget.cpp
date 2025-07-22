#include <lyric-tab/Widgets/LyricExtWidget.h>

#include <QFile>
#include <utility>

namespace FillLyric
{
    LyricExtWidget::LyricExtWidget(int *notesCount, LyricTabConfig config, QStringList priorityG2pIds,
                                   QWidget *parent) :
        QWidget(parent), notesCount(notesCount), m_priorityG2pIds(std::move(priorityG2pIds)) {
        this->setContentsMargins(0, 0, 0, 0);

        // phonicWidget
        m_wrapView = new LyricWrapView(":/tests/lyric-tab/Resources/theme/lyricwarpview-dark.qss");
        m_wrapView->setContentsMargins(0, 0, 0, 0);

        // tableTop layout
        m_tableTopLayout = new QHBoxLayout();
        m_tableTopLayout->setContentsMargins(0, 0, 0, 0);
        btnFoldLeft = new Button(tr("Fold Left"));

        m_btnInsertText = new Button(tr("Test"));
        m_tableTopLayout->addWidget(btnFoldLeft);
        m_tableTopLayout->addWidget(m_btnInsertText);
        m_tableTopLayout->addStretch(1);

        m_tableCountLayout = new QHBoxLayout();
        noteCountLabel = new QLabel("0/0");

        m_tableCountLayout->addStretch(1);
        m_tableCountLayout->addWidget(noteCountLabel);

        // table layout
        m_tableLayout = new QVBoxLayout();
        m_tableLayout->setContentsMargins(0, 0, 0, 0);
        m_tableLayout->addLayout(m_tableTopLayout);
        m_tableLayout->addWidget(m_wrapView);
        m_tableLayout->addLayout(m_tableCountLayout);

        m_mainLayout = new QHBoxLayout();
        m_mainLayout->setContentsMargins(0, 0, 0, 0);
        m_mainLayout->addLayout(m_tableLayout);
        this->setLayout(m_mainLayout);

        QFont font = m_wrapView->font();
        font.setPointSizeF(std::max(9.0, config.lyricExtFontSize));
        m_wrapView->setFont(font);

        // notes Count
        connect(m_wrapView, &LyricWrapView::noteCountChanged, this, &LyricExtWidget::_on_notesCountChanged);
        // view font size
        connect(m_wrapView, &LyricWrapView::fontSizeChanged, this, &LyricExtWidget::modifyOption);
    }

    LyricExtWidget::~LyricExtWidget() = default;

    void LyricExtWidget::_on_notesCountChanged(const int &count) const {
        noteCountLabel->setText(QString::number(count) + "/" + QString::number(*notesCount));
    }

} // namespace FillLyric
