#include "AddPrevLineCmd.h"

#include <lyric-tab/Controls/CellList.h>
#include <lyric-tab/Controls/LyricWrapView.h>

namespace FillLyric
{
    AddPrevLineCmd::AddPrevLineCmd(LyricWrapView *view, CellList *cellList, QUndoCommand *parent) :
        QUndoCommand(parent), m_view(view), m_newList(cellList) {
        m_index = static_cast<int>(m_view->cellLists().indexOf(cellList));
        m_newList = m_view->createNewList();
        m_newCell = m_newList->createNewCell();
        m_newList->removeFromScene();
    }

    void AddPrevLineCmd::undo() {
        m_newList->removeCell(m_newCell);
        m_view->removeList(m_index);
    }

    void AddPrevLineCmd::redo() {
        m_view->insertList(m_index, m_newList);
        m_newList->insertCell(0, m_newCell);
    }
} // namespace FillLyric
