#ifndef HANDLEITEM_H
#define HANDLEITEM_H

#include <QGraphicsItem>
#include <QGraphicsView>

#include <lyric-tab/LyricTabGlobal.h>

namespace FillLyric
{
    class LYRIC_TAB_EXPORT HandleItem final : public QObject, public QGraphicsItem {
        Q_OBJECT
        Q_INTERFACES(QGraphicsItem)
    public:
        explicit HandleItem(QGraphicsView *view, QGraphicsItem *parent = nullptr);
        ~HandleItem() override;

        qreal width() const;
        void setWidth(const qreal &w);

        qreal height() const;
        void setHeight(const qreal &h);

        qreal deltaX() const;

        void setMargin(const qreal &margin);
        qreal margin() const;

    Q_SIGNALS:
        void selectAll() const;
        void contextMenuRequested(const QPointF &pos) const;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

        void setQss();

        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    private:
        enum State {
            Normal = 0,
            Hovered = 1,
            Selected = 2,
        };

        QVector<QBrush> m_backgroundBrush = {QColor(83, 83, 85), QColor(137, 137, 139), QColor(112, 156, 255)};

        qreal mW = 13;
        qreal mH = 0;

        qreal m_margin = 4;
        QGraphicsView *m_view;
    };

} // namespace FillLyric

#endif // HANDLEITEM_H
