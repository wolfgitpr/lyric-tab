#ifndef SPLITTERGRAPHICSITEM_H
#define SPLITTERGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsView>

#include <lyric-tab/LyricTabGlobal.h>

namespace FillLyric
{
    class LYRIC_TAB_EXPORT SplitterItem final : public QGraphicsItem {
    public:
        explicit SplitterItem(const qreal &x, const qreal &y, const qreal &w, QGraphicsView *view,
                              QGraphicsItem *parent = nullptr);
        ~SplitterItem() override;

        qreal width() const;
        void setWidth(const qreal &w);

        qreal height() const;

        qreal deltaY() const;
        void setLineHeight(const qreal &lh);

        QPen pen() const;
        void setPen(const QPen &pen);

        void setMargin(const qreal &margin);
        qreal margin() const;

    protected:
        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        void setQss();

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    private:
        qreal mW = 0;

        qreal m_lineHeight = 1;

        QPen m_pen = QPen(QColor(120, 120, 120), 0.5);

        qreal m_margin = 5;

        QGraphicsView *m_view;
    };
} // namespace FillLyric

#endif // SPLITTERGRAPHICSITEM_H
