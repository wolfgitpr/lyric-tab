#ifndef ADDPREVCELLCMD_H
#define ADDPREVCELLCMD_H

#include <lyric-tab/Controls/CellList.h>
#include <lyric-tab/Controls/LyricCell.h>

namespace FillLyric
{

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
