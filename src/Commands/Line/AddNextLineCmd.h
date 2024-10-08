#ifndef ADDNEXTLINEEDIT_H
#define ADDNEXTLINEEDIT_H

#include <QUndoCommand>

namespace FillLyric
{
    class LyricCell;
    class CellList;
    class LyricWrapView;

    class AddNextLineCmd final : public QUndoCommand {
    public:
        explicit AddNextLineCmd(LyricWrapView *view, CellList *cellList, QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        LyricWrapView *m_view;
        int m_index;
        CellList *m_newList;
        LyricCell *m_newCell;
    };

} // namespace FillLyric

#endif // ADDNEXTLINEEDIT_H
