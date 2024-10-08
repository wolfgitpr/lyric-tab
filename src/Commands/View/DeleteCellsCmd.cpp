#include "DeleteCellsCmd.h"

#include <lyric-tab/Controls/CellList.h>
#include <lyric-tab/Controls/LyricCell.h>
#include <lyric-tab/Controls/LyricWrapView.h>

namespace FillLyric
{
    DeleteCellsCmd::DeleteCellsCmd(LyricWrapView *view, QList<LyricCell *> cells, QUndoCommand *parent) :
        QUndoCommand(parent), m_view(view), m_cells(std::move(cells)) {
        for (const auto &cell : m_cells) {
            if (const auto cellList = m_view->mapToList(cell->lyricRect().center().toPoint())) {
                m_cellsMap[cellList][static_cast<int>(cellList->m_cells.indexOf(cell))] = cell;
            }
        }

        for (const auto &cellList : m_cellsMap.keys()) {
            if (cellList->m_cells.size() == m_cellsMap[cellList].size())
                m_cellLists.append({m_view->cellLists().indexOf(cellList), cellList});
        }
    }

    void DeleteCellsCmd::undo() {
        for (const auto &cellList : m_cellLists) {
            if (!m_view->cellLists().contains(cellList.second))
                m_view->insertList(cellList.first, cellList.second);
        }

        const auto cellLists = m_cellsMap.keys();
        for (const auto &cellList : cellLists) {
            const auto indexes = m_cellsMap[cellList].keys();
            for (const auto &index : indexes) {
                cellList->insertCell(index, m_cellsMap[cellList][index]);
            }
        }
        m_view->repaintCellLists();
    }

    void DeleteCellsCmd::redo() {
        const auto cellLists = m_cellsMap.keys();
        for (const auto &cellList : cellLists) {
            const auto indexes = m_cellsMap[cellList].keys();
            for (const auto &index : indexes) {
                cellList->removeCell(m_cellsMap[cellList][index]);
            }
        }

        for (const auto &cellList : m_cellLists)
            m_view->removeList(cellList.second);

        m_view->repaintCellLists();
    }
} // namespace FillLyric
