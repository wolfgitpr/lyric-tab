#ifndef LYRICCELL_H
#define LYRICCELL_H

#include <QApplication>
#include <QObject>

#include <QGraphicsObject>
#include <QGraphicsView>

#include <language-manager/LangCommon.h>

#include <lyric-tab/LyricTabGlobal.h>

namespace FillLyric
{
    struct LYRIC_TAB_EXPORT CellQss {
        QVector<QBrush> cellBackgroundBrush = {Qt::NoBrush, QColor(255, 255, 255, 15), QColor(255, 255, 255, 30)};
        QVector<QPen> cellBorderPen = {QPen(QColor(83, 83, 85), 2), QPen(QColor(137, 137, 139), 2),
                                       QPen(QColor(155, 186, 255), 2)};

        QVector<QPen> cellLyricPen = {QColor(240, 240, 240), QColor(240, 240, 240), QColor(240, 240, 240),
                                      QColor(240, 240, 240)};
        QVector<QPen> cellSyllablePen = {QColor(240, 240, 240), QColor(155, 186, 255), QColor(255, 204, 153),
                                         QColor(255, 155, 157)};
    };

    class LYRIC_TAB_EXPORT LyricCell final : public QGraphicsObject {
        Q_OBJECT

    public:
        explicit LyricCell(const qreal &x, const qreal &y, LangNote *note, QGraphicsView *view, CellQss *qss,
                           QGraphicsItem *parent = nullptr);
        ~LyricCell() override;

        qreal width() const;
        qreal height() const;

        QPainterPath shape() const override;
        QRectF boundingRect() const override;

        QRectF lyricRect() const;
        QRectF syllableRect() const;

        LangNote *note() const;
        void setNote(LangNote *note);

        QString lyric() const;
        void setLyric(const QString &lyric);

        QString syllable() const;
        void setSyllable(const QString &syllable);

        qreal margin() const;
        void setMargin(const qreal &margin);

        void setFont(const QFont &font);
        void setLyricRect(const QRect &rect);
        void setSyllableRect(const QRect &rect);

    Q_SIGNALS:
        void updateLyric(FillLyric::LyricCell *cell, const QString &lyric) const;
        void changeSyllable(FillLyric::LyricCell *cell, const QString &syllable) const;

        void clearCell(FillLyric::LyricCell *cell) const;
        void deleteCell(FillLyric::LyricCell *cell) const;
        void addPrevCell(FillLyric::LyricCell *cell) const;
        void addNextCell(FillLyric::LyricCell *cell) const;
        void linebreak(FillLyric::LyricCell *cell) const;

        void updateWidth(const qreal &w) const;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    private:
        void updateLyricRect();
        void setQss(CellQss *qss);

        qreal lyricWidth() const;
        qreal syllableWidth() const;

        QPointF lyricPos() const;
        QPointF rectPos() const;
        QPointF syllablePos() const;

        void changePhonicMenu(QMenu *menu);
        void changeSyllableMenu(QMenu *menu);

        QRect m_lRect;
        QRect m_sRect;
        CellQss *m_qss;

        LangNote *m_note;
        QGraphicsView *m_view;

        qreal m_lsPadding = 5;
        qreal m_rectPadding = 3;
        qreal m_padding = 3;
        qreal m_reckBorder = 2.5;

        QFont m_font = QApplication::font();

    private:
        enum State {
            Normal = 0,
            Hovered = 1,
            Selected = 2,
        };

        QVector<QBrush> m_backgroundBrush = {Qt::NoBrush, QColor(255, 255, 255, 15), QColor(255, 255, 255, 30)};
        QVector<QPen> m_borderPen = {QPen(QColor(83, 83, 85), 2), QPen(QColor(137, 137, 139), 2),
                                     QPen(QColor(155, 186, 255), 2)};

        enum PenType { MultiTone = 1, Revised, G2pError };

        QVector<QPen> m_lyricPen = {QColor(240, 240, 240), QColor(240, 240, 240), QColor(240, 240, 240),
                                    QColor(240, 240, 240)};
        QVector<QPen> m_syllablePen = {QColor(240, 240, 240), QColor(155, 186, 255), QColor(255, 204, 153),
                                       QColor(255, 155, 157)};
    };
} // namespace FillLyric

#endif // LYRICCELL_H
