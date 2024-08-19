#ifndef DELETECELL_H
#define DELETECELL_H

#include <lyric-tab/Controls/CellList.h>

namespace FillLyric {

    class DeleteCellCmd final : public QUndoCommand {
    public:
        explicit DeleteCellCmd(CellList *cellList, LyricCell *cell, QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        CellList *m_list;
        int m_index;
        LyricCell *m_cell;
    };

} // FillLyric

#endif // DELETECELL_H
