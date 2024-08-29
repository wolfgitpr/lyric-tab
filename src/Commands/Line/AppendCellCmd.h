#ifndef APPENDCELLCMD_H
#define APPENDCELLCMD_H

#include <QUndoCommand>

namespace FillLyric
{
    class LyricCell;
    class CellList;

    class AppendCellCmd final : public QUndoCommand {
    public:
        explicit AppendCellCmd(CellList *cellList, QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        CellList *m_list;
        LyricCell *m_newCell;
    };

} // namespace FillLyric

#endif // APPENDCELLCMD_H
