#include <lyric-tab/Widgets/LyricExtWidget.h>

#include <QFile>

namespace FillLyric
{
    LyricExtWidget::LyricExtWidget(int *notesCount, LyricTabConfig config, QWidget *parent) :
        QWidget(parent), notesCount(notesCount) {
        this->setContentsMargins(0, 0, 0, 0);

        // phonicWidget
        m_wrapView = new LyricWrapView(":/tests/Resources/theme/lyricwarpview-dark.qss");
        m_wrapView->setContentsMargins(0, 0, 0, 0);

        // tableTop layout
        m_tableTopLayout = new QHBoxLayout();
        m_tableTopLayout->setContentsMargins(0, 0, 0, 0);
        btnFoldLeft = new Button(tr("Fold Left"));

        autoWrapLabel = new QLabel(tr("Auto Wrap"));
        autoWrap = new SwitchButton();

        btnUndo = new QPushButton();
        btnUndo->setShortcut(QKeySequence("Ctrl+Z"));
        btnUndo->setEnabled(false);
        btnUndo->setMinimumSize(24, 24);
        btnUndo->setFixedWidth(24);
        btnUndo->setIcon(QIcon(":/tests/Resources/svg/icons/arrow_undo_16_filled_white.svg"));

        btnRedo = new QPushButton();
        btnRedo->setShortcut(QKeySequence("Ctrl+Y"));
        btnRedo->setEnabled(false);
        btnRedo->setMinimumSize(24, 24);
        btnRedo->setFixedWidth(24);
        btnRedo->setIcon(QIcon(":/tests/Resources/svg/icons/arrow_redo_16_filled_white.svg"));

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

        // export option
        m_epOptLabelLayout = new QHBoxLayout();
        exportOptLabel = new QLabel(tr("Export Option:"));
        exportOptButton = new QPushButton();
        exportOptButton->setFixedSize(20, 20);
        exportOptButton->setIcon(QIcon(":/tests/Resources/svg/icons/chevron_down_16_filled_white.svg"));

        m_epOptLabelLayout->addWidget(exportOptLabel);
        m_epOptLabelLayout->addStretch(1);
        m_epOptLabelLayout->addWidget(exportOptButton);

        // export option layout
        m_epOptWidget = new QWidget();
        m_epOptLayout = new QVBoxLayout();
        m_epOptLayout->setContentsMargins(0, 0, 0, 0);
        exportLanguage = new QCheckBox(tr("Automatically mark languages"));

        m_epOptLayout->addWidget(exportLanguage);
        m_epOptLayout->addStretch(1);

        m_epOptWidget->setVisible(false);
        m_epOptWidget->setContentsMargins(0, 0, 0, 0);
        m_epOptWidget->setLayout(m_epOptLayout);

        // table layout
        m_tableLayout = new QVBoxLayout();
        m_tableLayout->setContentsMargins(0, 0, 0, 0);
        m_tableLayout->addLayout(m_tableTopLayout);
        m_tableLayout->addWidget(m_wrapView);
        m_tableLayout->addLayout(m_tableCountLayout);
        m_tableLayout->addLayout(m_epOptLabelLayout);
        m_tableLayout->addWidget(m_epOptWidget);

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

        exportLanguage->setChecked(config.exportLanguage);

        // undo redo
        m_history = m_wrapView->history();
        connect(btnUndo, &QPushButton::clicked,
                [this]()
                {
                    m_wrapView->history()->undo();
                    m_wrapView->updateCellRect();
                });
        connect(btnRedo, &QPushButton::clicked,
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

        // exportOptButton
        connect(exportOptButton, &QPushButton::clicked,
                [this]() { m_epOptWidget->setVisible(!m_epOptWidget->isVisible()); });

        // view font size
        connect(m_wrapView, &LyricWrapView::fontSizeChanged, this, &LyricExtWidget::modifyOption);

        connect(autoWrap, &SwitchButton::clicked, this, &LyricExtWidget::modifyOption);
        connect(exportLanguage, &QCheckBox::stateChanged, this, &LyricExtWidget::modifyOption);
    }

    LyricExtWidget::~LyricExtWidget() = default;

    void LyricExtWidget::_on_notesCountChanged(const int &count) const {
        noteCountLabel->setText(QString::number(count) + "/" + QString::number(*notesCount));
    }

} // namespace FillLyric
