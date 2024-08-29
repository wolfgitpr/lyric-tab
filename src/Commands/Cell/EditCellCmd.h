#ifndef EDITCELLCMD_H
#define EDITCELLCMD_H

#include <QUndoCommand>

#include <language-manager/LangCommon.h>

namespace FillLyric
{
    class LyricCell;
    class CellList;

    class EditCellCmdfinal final : public QUndoCommand {
    public:
        explicit EditCellCmdfinal(CellList *cellList, LyricCell *cell, const QString &lyric,
                                  QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        CellList *m_list;
        LyricCell *m_cell;
        qlonglong m_index;

        LangNote *m_oldNote;
        LangNote *m_newNote;
    };


} // namespace FillLyric

#endif // EDITCELLCMD_H
