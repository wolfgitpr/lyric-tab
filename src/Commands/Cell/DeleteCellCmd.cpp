#include "DeleteCellCmd.h"

#include <lyric-tab/Controls/CellList.h>

namespace FillLyric
{
    DeleteCellCmd::DeleteCellCmd(CellList *cellList, LyricCell *cell, QUndoCommand *parent) :
        QUndoCommand(parent), m_list(cellList), m_cell(cell) {
        m_index = static_cast<int>(m_list->m_cells.indexOf(cell));
    }

    void DeleteCellCmd::undo() {
        m_list->insertCell(m_index, m_cell);
        m_list->updateCellPos();
    }

    void DeleteCellCmd::redo() {
        m_list->removeCell(m_cell);
        m_list->updateCellPos();
    }
} // namespace FillLyric
