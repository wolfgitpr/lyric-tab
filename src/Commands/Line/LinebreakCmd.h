#ifndef LINEBREAKCMD_H
#define LINEBREAKCMD_H

#include <QUndoCommand>

namespace FillLyric
{
    class LyricCell;
    class CellList;
    class LyricWrapView;

    class LinebreakCmd final : public QUndoCommand {
    public:
        explicit LinebreakCmd(LyricWrapView *view, CellList *cellList, const int &index,
                              QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        LyricWrapView *m_view;
        int m_index;
        int m_cellListIndex;
        CellList *m_list;
        CellList *m_newList;
        QList<LyricCell *> m_cells;
    };

} // namespace FillLyric

#endif // LINEBREAKCMD_H
