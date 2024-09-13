#ifndef ADDPREVLINECMD_H
#define ADDPREVLINECMD_H

#include <QUndoCommand>

namespace FillLyric
{
    class LyricCell;
    class CellList;
    class LyricWrapView;

    class AddPrevLineCmd final : public QUndoCommand {
    public:
        explicit AddPrevLineCmd(LyricWrapView *view, CellList *cellList, QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        LyricWrapView *m_view;
        int m_index;
        CellList *m_newList;
        LyricCell *m_newCell;
    };

} // namespace FillLyric

#endif // ADDPREVLINECMD_H
