#include <lyric-tab/Utils/QssParser.h>

#include <QGraphicsView>

namespace FillLyric
{
    QString QssParser::propertyValue(const QGraphicsView *view, const QString &propertyName) {
        const auto list = view->property(propertyName.toUtf8()).toStringList();
        if (list.size() < 2)
            return {};
        return list[1];
    }

    QVector<QBrush> QssParser::parseBrushes(const QString &value, int count) {
        QVector<QBrush> result;
        if (value.isEmpty())
            return result;

        const auto brushList = value.split('|');
        if (brushList.size() != count)
            return result;

        for (int i = 0; i < count; i++) {
            if (brushList[i] == "NoBrush") {
                result.append(QBrush(Qt::NoBrush));
            } else {
                const auto colorStr = brushList[i].split(',');
                if (colorStr.size() == 4) {
                    result.append(QBrush(QColor(colorStr[0].toInt(), colorStr[1].toInt(),
                                                colorStr[2].toInt(), colorStr[3].toInt())));
                }
            }
        }
        return result;
    }

    QVector<QPen> QssParser::parsePens(const QString &value, int count) {
        QVector<QPen> result;
        if (value.isEmpty())
            return result;

        const auto penListStr = value.split('|');
        if (penListStr.size() != count)
            return result;

        for (const auto &pen : penListStr) {
            const auto penValue = pen.split(',');
            if (penValue.size() == 4)
                result.append(QPen(QColor(penValue[0].toInt(), penValue[1].toInt(),
                                          penValue[2].toInt(), penValue[3].toInt())));
            else if (penValue.size() == 5)
                result.append(QPen(QColor(penValue[0].toInt(), penValue[1].toInt(),
                                          penValue[2].toInt(), penValue[3].toInt()),
                                   penValue[4].toInt()));
        }
        return result;
    }

    QPen QssParser::parsePen(const QString &value) {
        if (value.isEmpty())
            return {};

        const auto colorStr = value.split(',');
        if (colorStr.size() == 5) {
            return QPen(QColor(colorStr[0].toInt(), colorStr[1].toInt(),
                               colorStr[2].toInt(), colorStr[3].toInt()),
                        colorStr[4].toInt());
        }
        return {};
    }
} // namespace FillLyric
