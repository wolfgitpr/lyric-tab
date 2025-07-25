#ifndef DS_EDITOR_LITE_CLEANLYRIC_H
#define DS_EDITOR_LITE_CLEANLYRIC_H

#include <language-manager/LangCommon.h>

namespace FillLyric
{
    class CleanLyric {
    public:
        static QList<QList<LangNote>> splitAuto(const QString &input, const QStringList &priorityG2pIds);

        static QList<QList<LangNote>> splitByChar(const QString &input);

        static QList<QList<LangNote>> splitCustom(const QString &input, const QStringList &splitter);
    };
} // namespace FillLyric


#endif // DS_EDITOR_LITE_CLEANLYRIC_H
