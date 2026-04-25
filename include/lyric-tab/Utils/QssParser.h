#ifndef LYRIC_TAB_UTILS_QSS_PARSER_H
#define LYRIC_TAB_UTILS_QSS_PARSER_H

#include <QBrush>
#include <QGraphicsView>
#include <QPen>
#include <QStringList>
#include <QVector>

#include <lyric-tab/LyricTabGlobal.h>

namespace FillLyric
{
    namespace QssParser
    {
        LYRIC_TAB_EXPORT QVector<QBrush> parseBrushes(const QString &value, int count);
        LYRIC_TAB_EXPORT QVector<QPen> parsePens(const QString &value, int count);
        LYRIC_TAB_EXPORT QPen parsePen(const QString &value);
        LYRIC_TAB_EXPORT QString propertyValue(const QGraphicsView *view, const QString &propertyName);
    } // namespace QssParser
} // namespace FillLyric

#endif // LYRIC_TAB_UTILS_QSS_PARSER_H
