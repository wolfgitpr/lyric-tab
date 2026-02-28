#ifndef LYRIC_TAB_LANGCOMMON_H
#define LYRIC_TAB_LANGCOMMON_H

#include <QString>
#include <QStringList>

#include <lyric-tab/LyricTabGlobal.h>

struct LYRIC_TAB_EXPORT LangNote {
    QString lyric;
    QString syllable = QString();
    QString syllableRevised = QString();
    QStringList candidates = QStringList();
    QString standardG2pId = "unknown";
    QString g2pId = "unknown";
    QString language = "unknown";
    bool revised = false;
    bool error = false;

    LangNote() = default;

    explicit LangNote(QString lyric) : lyric(std::move(lyric)) {}
};
#endif // LYRIC_TAB_LANGCOMMON_H
