#include "LinebreakCmd.h"

#include <lyric-tab/Controls/CellList.h>
#include <lyric-tab/Controls/LyricWrapView.h>

namespace FillLyric
{
    LinebreakCmd::LinebreakCmd(LyricWrapView *view, CellList *cellList, const int &index, QUndoCommand *parent) :
        QUndoCommand(parent), m_view(view), m_index(index), m_list(cellList) {
        m_cellListIndex = static_cast<int>(m_view->cellLists().indexOf(m_list));
        m_cells = m_list->m_cells.mid(index);
        m_newList = m_view->createNewList();
    }

    void LinebreakCmd::undo() {
        for (const auto &cell : m_cells) {
            m_list->disconnectCell(cell);
            m_newList->removeCell(cell);

            m_list->connectCell(cell);
            m_list->appendCell(cell);
        }
        m_view->removeList(m_newList);
        m_view->repaintCellLists();
    }

    void LinebreakCmd::redo() {
        m_view->insertList(m_cellListIndex + 1, m_newList);
        for (const auto &cell : m_cells) {
            m_list->disconnectCell(cell);
            m_list->removeCell(cell);

            m_newList->connectCell(cell);
            m_newList->appendCell(cell);
        }
        m_view->repaintCellLists();
    }
} // namespace FillLyric
