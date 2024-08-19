#ifndef LYRICTABCONFIG_H
#define LYRICTABCONFIG_H

#include <lyric-tab/LyricTabGlobal.h>

namespace FillLyric
{
    struct LYRIC_TAB_EXPORT LyricTabConfig {
        // lyricTab
        bool lyricBaseVisible = true;
        bool lyricExtVisible = false;

        // lyricBase
        double lyricBaseFontSize = 11;
        bool baseSkipSlur = false;
        int splitMode = 0;

        // lyricExt
        double lyricExtFontSize = 12;
        bool autoWrap = true;
        bool exportLanguage = false;
    };
} // namespace FillLyric
#endif // LYRICTABCONFIG_H
