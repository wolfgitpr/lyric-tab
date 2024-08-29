#ifndef DELETELINECMD_H
#define DELETELINECMD_H

#include <QUndoCommand>

namespace FillLyric
{
    class CellList;
    class LyricWrapView;

    class DeleteLineCmd final : public QUndoCommand {
    public:
        explicit DeleteLineCmd(LyricWrapView *view, CellList *cellList, QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        LyricWrapView *m_view;
        int m_index;
        CellList *m_list;
    };

} // namespace FillLyric

#endif // DELETELINECMD_H
