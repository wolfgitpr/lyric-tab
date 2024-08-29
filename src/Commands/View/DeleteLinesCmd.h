#ifndef DELETELINES_H
#define DELETELINES_H

#include <QMap>
#include <QUndoCommand>

namespace FillLyric
{
    class CellList;
    class LyricWrapView;

    class DeleteLinesCmd final : public QUndoCommand {
    public:
        explicit DeleteLinesCmd(LyricWrapView *view, const QList<CellList *> &lists, QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        LyricWrapView *m_view;
        QMap<int, CellList *> m_listMap;
    };

} // namespace FillLyric

#endif // DELETELINES_H
