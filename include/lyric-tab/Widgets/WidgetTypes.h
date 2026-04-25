#ifndef LYRIC_TAB_WIDGETS_WIDGET_TYPES_H
#define LYRIC_TAB_WIDGETS_WIDGET_TYPES_H

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

namespace FillLyric
{
#ifndef USE_LITE_CONTROLS
    using Button = QPushButton;
    using ComboBox = QComboBox;
    using LineEdit = QLineEdit;
    using SwitchButton = QCheckBox;
#endif
}

#endif // LYRIC_TAB_WIDGETS_WIDGET_TYPES_H
