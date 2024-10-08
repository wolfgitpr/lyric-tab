#ifndef CLEARCELLCMD_H
#define CLEARCELLCMD_H

#include <QUndoCommand>

namespace FillLyric
{
    class LyricCell;
    class CellList;

    class ClearCellCmd final : public QUndoCommand {
    public:
        explicit ClearCellCmd(CellList *cellList, LyricCell *cell, QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        CellList *m_list;
        int m_index;

        LyricCell *m_cell;
        LyricCell *m_newCell;
    };

} // namespace FillLyric

#endif // CLEARCELLCMD_H
