#ifndef DELETECELLSCMD_H
#define DELETECELLSCMD_H

#include <QMap>
#include <QUndoCommand>

namespace FillLyric
{
    class LyricCell;
    class CellList;
    class LyricWrapView;

    class DeleteCellsCmd final : public QUndoCommand {
    public:
        explicit DeleteCellsCmd(LyricWrapView *view, QList<LyricCell *> cells, QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        LyricWrapView *m_view;
        QMap<CellList *, QMap<int, LyricCell *>> m_cellsMap;
        QList<LyricCell *> m_cells;
        QList<QPair<int, CellList *>> m_cellLists;
    };

} // namespace FillLyric

#endif // DELETECELLSCMD_H
