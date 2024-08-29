#ifndef DELETECELL_H
#define DELETECELL_H

#include <QUndoCommand>

namespace FillLyric
{
    class LyricCell;
    class CellList;

    class DeleteCellCmd final : public QUndoCommand {
    public:
        explicit DeleteCellCmd(CellList *cellList, LyricCell *cell, QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        CellList *m_list;
        int m_index;
        LyricCell *m_cell;
    };

} // namespace FillLyric

#endif // DELETECELL_H
