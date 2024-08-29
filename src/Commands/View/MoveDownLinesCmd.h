#ifndef MOVEDOWNLINESCMD_H
#define MOVEDOWNLINESCMD_H

#include <QUndoCommand>

namespace FillLyric
{
    class CellList;
    class LyricWrapView;

    class MoveDownLinesCmd final : public QUndoCommand {
    public:
        explicit MoveDownLinesCmd(LyricWrapView *view, const QList<CellList *> &lists, QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        LyricWrapView *m_view;
        QList<CellList *> m_lists;
    };

} // namespace FillLyric

#endif // MOVEDOWNLINESCMD_H
