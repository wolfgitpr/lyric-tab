#include <lyric-tab/Controls/SplitterItem.h>

#include <QPainter>

namespace FillLyric
{
    SplitterItem::SplitterItem(const qreal &x, const qreal &y, const qreal &w, QGraphicsView *view,
                               QGraphicsItem *parent) : QGraphicsItem(parent), m_view(view) {
        this->setX(x);
        this->setY(y);
        this->setWidth(w);

        this->setQss();
    }

    SplitterItem::~SplitterItem() = default;

    QRectF SplitterItem::boundingRect() const {
        if (this->isVisible())
            return {0, 0, width(), height()};
        else
            return {0, 0, 0, m_margin / 2};
    }

    QPainterPath SplitterItem::shape() const {
        QPainterPath path;
        if (this->isVisible())
            path.addRect({0, m_margin, width(), m_lineHeight});
        else
            path.addRect({0, 0, 0, m_margin / 2});
        return path;
    }

    qreal SplitterItem::width() const { return mW; }

    void SplitterItem::setWidth(const qreal &w) {
        mW = w;
        update();
    }

    qreal SplitterItem::height() const {
        if (this->isVisible())
            return m_lineHeight + m_margin * 2;
        else
            return m_margin / 2;
    }

    qreal SplitterItem::deltaY() const {
        if (this->isVisible())
            return m_lineHeight + m_margin + 1;
        else
            return m_margin / 2;
    }

    void SplitterItem::setLineHeight(const qreal &lh) {
        m_lineHeight = lh + m_margin * 2;
        update();
    }

    QPen SplitterItem::pen() const { return m_pen; }

    void SplitterItem::setPen(const QPen &pen) {
        if (m_pen != pen) {
            m_pen = pen;
            update();
        }
    }

    void SplitterItem::setMargin(const qreal &margin) {
        if (m_margin != margin) {
            m_margin = margin;
            update();
        }
    }

    qreal SplitterItem::margin() const { return m_margin; }

    void SplitterItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        painter->setPen(m_pen);
        for (int i = 0; i < m_lineHeight; i++) {
            painter->drawLine(QPointF(m_margin, m_margin + i), QPointF(mW, m_margin + i));
        }
    }

    void SplitterItem::setQss() {
        const auto cellBackBrush = m_view->property("spliterPen").toStringList()[1];
        if (!cellBackBrush.isEmpty()) {
            const auto colorStr = cellBackBrush.split(',');

            if (colorStr.size() == 5) {
                m_pen = QPen(QColor(colorStr[0].toInt(), colorStr[1].toInt(), colorStr[2].toInt(), colorStr[3].toInt()),
                             colorStr[4].toInt());
            }
        }
    }
} // namespace FillLyric
