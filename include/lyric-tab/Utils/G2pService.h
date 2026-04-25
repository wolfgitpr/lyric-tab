#ifndef LYRIC_TAB_UTILS_G2P_SERVICE_H
#define LYRIC_TAB_UTILS_G2P_SERVICE_H

#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>
#include <memory>
#include <string>
#include <vector>

#include <lyric-tab/LangCommon.h>
#include <lyric-tab/LyricTabGlobal.h>

namespace FillLyric
{
    struct G2pResult {
        QString language;
        QString g2pId;
        QString syllable;
        QStringList candidates;
    };

    class LYRIC_TAB_EXPORT G2pService {
    public:
        static QList<G2pResult> convert(const QList<LangNote> &notes,
                                        const std::vector<std::string> &priorityG2pIds = {},
                                        const QMap<std::string, std::string> &langToG2pId = {});

        static G2pResult convertSingle(const LangNote &note, const std::vector<std::string> &priorityG2pIds = {},
                                       const QMap<std::string, std::string> &langToG2pId = {});
    };
} // namespace FillLyric

#endif // LYRIC_TAB_UTILS_G2P_SERVICE_H
