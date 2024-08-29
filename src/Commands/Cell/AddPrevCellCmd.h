#ifndef ADDPREVCELLCMD_H
#define ADDPREVCELLCMD_H

#include <QUndoCommand>

namespace FillLyric
{
    class LyricCell;
    class CellList;

    class AddPrevCellCmd final : public QUndoCommand {
    public:
        explicit AddPrevCellCmd(CellList *cellList, LyricCell *cell, QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        CellList *m_list;
        int m_index;
        LyricCell *m_newCell;
    };

} // namespace FillLyric

#endif // ADDPREVCELLCMD_H
