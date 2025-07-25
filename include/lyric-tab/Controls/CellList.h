#ifndef CELLLIST_H
#define CELLLIST_H

#include <QApplication>
#include <QTimer>

#include <language-manager/LangCommon.h>

#include <lyric-tab/Controls/HandleItem.h>
#include <lyric-tab/Controls/SplitterItem.h>

#include <lyric-tab/LyricTabGlobal.h>

namespace FillLyric
{
    class CellQss;
    class LyricCell;

    class LYRIC_TAB_EXPORT CellList final : public QGraphicsObject {
        Q_OBJECT

    public:
        explicit CellList(const qreal &x, const qreal &y, const QList<LangNote *> &noteList, QGraphicsScene *scene,
                          QGraphicsView *view, QList<CellList *> *cellLists);

        void clear();

        qreal deltaX() const;

        qreal deltaY() const;

        void setBaseY(const qreal &y);

        qreal height() const;
        qreal cellWidth() const;

        QGraphicsView *view() const;

        LyricCell *createNewCell();

        void highlight();
        void selectCells(const QPointF &startPos, const QPointF &endPos);

        void appendCell(LyricCell *cell);
        void removeCell(LyricCell *cell);
        void insertCell(const int &index, LyricCell *cell);

        void addToScene();
        void removeFromScene();

        void setWidth(const qreal &width);
        void updateSplitter(const qreal &width);

        void setFont(const QFont &font);
        void updateRect(LyricCell *cell);
        void updateCellPos();

        void connectCell(const LyricCell *cell) const;
        void disconnectCell(const LyricCell *cell) const;

        QList<LyricCell *> m_cells;

    Q_SIGNALS:
        void heightChanged() const;
        void cellPosChanged() const;

        void deleteLine() const;
        void addPrevLine() const;
        void addNextLine() const;
        void moveUpLine() const;
        void moveDownLine() const;

        void linebreakSignal(const int &cellIndex) const;

    public Q_SLOTS:
        void selectList() const;

    protected:
        QRectF boundingRect() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    private:
        void showContextMenu(const QPointF &pos);
        void setCellQss() const;
        QVector<QPen> qssPens(const QString &property) const;

        qreal m_curWidth = 0;
        qreal m_height = 0;
        qreal m_cellMargin = 5;

        QFont m_font = QApplication::font();
        QGraphicsView *m_view;
        QGraphicsScene *m_scene;

        SplitterItem *m_splitter;
        HandleItem *m_handle;

        CellQss *m_cellQss;
        QList<CellList *> *m_cellLists;

    private Q_SLOTS:
        void editCell(FillLyric::LyricCell *cell, const QString &lyric);
        void changeSyllable(FillLyric::LyricCell *cell, const QString &syllable);
        static void clearCell(FillLyric::LyricCell *cell);
        void deleteCell(FillLyric::LyricCell *cell);
        void addPrevCell(FillLyric::LyricCell *cell);
        void addNextCell(FillLyric::LyricCell *cell);
        void linebreak(FillLyric::LyricCell *cell) const;
    };
} // namespace FillLyric

#endif // CELLLIST_H
