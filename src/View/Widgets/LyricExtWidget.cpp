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

        autoWrapLabel = new QLabel(tr("Auto Wrap"));
        autoWrap = new SwitchButton();
        autoWrapLabel->setBuddy(autoWrap);

        btnUndo = new QPushButton();
        btnUndo->setToolTip(tr("Undo"));
        btnUndo->setShortcut(QKeySequence("Ctrl+Z"));
        btnUndo->setEnabled(false);
        btnUndo->setMinimumSize(24, 24);
        btnUndo->setFixedWidth(24);
        btnUndo->setIcon(QIcon(":/tests/lyric-tab/Resources/svg/icons/arrow_undo_16_filled_white.svg"));

        btnRedo = new QPushButton();
        btnRedo->setToolTip(tr("Redo"));
        btnRedo->setShortcut(QKeySequence("Ctrl+Y"));
        btnRedo->setEnabled(false);
        btnRedo->setMinimumSize(24, 24);
        btnRedo->setFixedWidth(24);
        btnRedo->setIcon(QIcon(":/tests/lyric-tab/Resources/svg/icons/arrow_redo_16_filled_white.svg"));

        m_btnInsertText = new Button(tr("Test"));
        m_tableTopLayout->addWidget(btnFoldLeft);
        m_tableTopLayout->addWidget(btnUndo);
        m_tableTopLayout->addWidget(btnRedo);
        m_tableTopLayout->addWidget(m_btnInsertText);
        m_tableTopLayout->addStretch(1);
        m_tableTopLayout->addWidget(autoWrapLabel);
        m_tableTopLayout->addWidget(autoWrap);

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

#ifdef LITE_CONTROLS
        autoWrap->setValue(config.autoWrap);
        m_wrapView->setAutoWrap(config.autoWrap);
#else
        autoWrap->setChecked(config.autoWrap);
        m_wrapView->setAutoWrap(config.autoWrap);
#endif

        QFont font = m_wrapView->font();
        font.setPointSizeF(std::max(9.0, config.lyricExtFontSize));
        m_wrapView->setFont(font);

        // undo redo
        m_history = m_wrapView->history();
        connect(btnUndo, &QPushButton::clicked, this,
                [this]()
                {
                    m_wrapView->history()->undo();
                    m_wrapView->updateCellRect();
                });
        connect(btnRedo, &QPushButton::clicked, this,
                [this]()
                {
                    m_wrapView->history()->redo();
                    m_wrapView->updateCellRect();
                });
        connect(m_history, &QUndoStack::canUndoChanged, btnUndo, &QPushButton::setEnabled);
        connect(m_history, &QUndoStack::canRedoChanged, btnRedo, &QPushButton::setEnabled);

        connect(autoWrap, &QCheckBox::clicked, m_wrapView, &LyricWrapView::setAutoWrap);

        // notes Count
        connect(m_wrapView, &LyricWrapView::noteCountChanged, this, &LyricExtWidget::_on_notesCountChanged);
        // view font size
        connect(m_wrapView, &LyricWrapView::fontSizeChanged, this, &LyricExtWidget::modifyOption);

        connect(autoWrap, &SwitchButton::clicked, this, &LyricExtWidget::modifyOption);
    }

    LyricExtWidget::~LyricExtWidget() = default;

    void LyricExtWidget::_on_notesCountChanged(const int &count) const {
        noteCountLabel->setText(QString::number(count) + "/" + QString::number(*notesCount));
    }

} // namespace FillLyric
