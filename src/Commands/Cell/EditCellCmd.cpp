#include "EditCellCmd.h"

#include <language-manager/ILanguageManager.h>

#include <lyric-tab/Controls/CellList.h>
#include <lyric-tab/Controls/LyricCell.h>

#include "lyric-tab/Controls/LyricWrapView.h"

namespace FillLyric
{
    EditCellCmdfinal::EditCellCmdfinal(LyricWrapView *view, CellList *cellList, LyricCell *cell, const QString &lyric,
                                       QUndoCommand *parent) :
        QUndoCommand(parent), m_view(view), m_list(cellList), m_cell(cell) {
        m_oldNote = cell->note();
        m_index = m_list->m_cells.indexOf(cell);
        const auto langMgr = LangMgr::ILanguageManager::instance();

        // TODO: update all notes
        QList<LangNote *> tempNotes;
        for (const auto &lyricCell : m_list->m_cells) {
            tempNotes.append(new LangNote(*lyricCell->note()));
        }
        tempNotes[m_index]->lyric = lyric;
        tempNotes[m_index]->g2pId = langMgr->analysis(lyric, view->priorityG2pIds());

        langMgr->convert(tempNotes);

        m_newNote = new LangNote(*tempNotes.at(m_index));
    }

    void EditCellCmdfinal::undo() {
        m_cell->setNote(m_oldNote);
        m_list->updateRect(m_cell);
        m_cell->update();
    }

    void EditCellCmdfinal::redo() {
        m_cell->setNote(m_newNote);
        m_list->updateRect(m_cell);
        m_cell->update();
    }
} // namespace FillLyric
