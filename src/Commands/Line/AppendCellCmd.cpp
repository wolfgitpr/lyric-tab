#include "AppendCellCmd.h"

#include <lyric-tab/Controls/CellList.h>

namespace FillLyric
{
    AppendCellCmd::AppendCellCmd(CellList *cellList, QUndoCommand *parent) : QUndoCommand(parent), m_list(cellList) {
        m_newCell = m_list->createNewCell();
    }

    void AppendCellCmd::undo() { m_list->removeCell(m_newCell); }

    void AppendCellCmd::redo() { m_list->appendCell(m_newCell); }
} // namespace FillLyric
