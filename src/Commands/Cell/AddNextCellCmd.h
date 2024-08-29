#ifndef ADDNEXTCELLCMD_H
#define ADDNEXTCELLCMD_H

#include <QUndoCommand>

namespace FillLyric
{
    class LyricCell;
    class CellList;

    class AddNextCellCmd final : public QUndoCommand {
    public:
        explicit AddNextCellCmd(CellList *cellList, LyricCell *cell, QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        CellList *m_list;
        int m_index;
        LyricCell *m_newCell;
    };
} // namespace FillLyric

#endif // ADDNEXTCELLCMD_H
