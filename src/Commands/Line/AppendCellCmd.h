#ifndef APPENDCELLCMD_H
#define APPENDCELLCMD_H

#include <lyric-tab/Controls/LyricWrapView.h>

namespace FillLyric
{

    class AppendCellCmd final : public QUndoCommand {
    public:
        explicit AppendCellCmd(LyricWrapView *view, CellList *cellList, QUndoCommand *parent = nullptr);
        void undo() override;
        void redo() override;

    private:
        LyricWrapView *m_view;
        CellList *m_list;
        LyricCell *m_newCell;
    };

} // namespace FillLyric

#endif // APPENDCELLCMD_H
