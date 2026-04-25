#ifndef LYRIC_TAB_LANG_COMMON_H
#define LYRIC_TAB_LANG_COMMON_H

#include <QString>
#include <QStringList>

#include <lyric-tab/LyricTabGlobal.h>

struct LYRIC_TAB_EXPORT LangNote {
    QString lyric;
    QString syllable = QString();
    QString syllableRevised = QString();
    QStringList candidates = QStringList();
    QString language = "unknown";
    QString g2pId = "unknown";
    bool revised = false;
    bool error = false;

    LangNote() = default;

    explicit LangNote(QString lyric) : lyric(std::move(lyric)) {}
    explicit LangNote(QString lyric, QString language, QString g2pId) :
        lyric(std::move(lyric)), language(std::move(language)), g2pId(std::move(g2pId)) {}
};
#endif // LYRIC_TAB_LANG_COMMON_H
