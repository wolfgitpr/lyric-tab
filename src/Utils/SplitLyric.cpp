#include "SplitLyric.h"
#include <language-manager/ILanguageManager.h>

namespace FillLyric
{
    QList<QList<LangNote>> CleanLyric::splitAuto(const QString &input, const QStringList &priorityG2pIds) {
        QList<QList<LangNote>> result;
        QList<LangNote> notes;
        const auto langMgr = LangMgr::ILanguageManager::instance();
        const auto res = langMgr->split(input, priorityG2pIds);

        for (const auto &note : res) {
            if (note.g2pId == "linebreak") {
                if (!notes.isEmpty())
                    result.append(notes);
                notes.clear();
                continue;
            }
            notes.append(note);
        }

        if (!notes.isEmpty())
            result.append(notes);
        return result;
    }

    static bool containLinebreak(const QChar &c) {
        return c == QChar::LineFeed || c == QChar::LineSeparator || c == QChar::ParagraphSeparator;
    }

    static bool containLinebreak(const QString &input) {
        if (input.size() == 1)
            return containLinebreak(input.at(0));
        return false;
    }

    QList<QList<LangNote>> CleanLyric::splitByChar(const QString &input) {
        const auto langMgr = LangMgr::ILanguageManager::instance();

        QList<QList<LangNote>> result;
        QList<LangNote> notes;
        for (int i = 0; i < input.length(); i++) {
            const QChar &currentChar = input[i];
            if (currentChar == ' ') {
                continue;
            }
            if (containLinebreak(currentChar)) {
                if (!notes.isEmpty())
                    result.append(notes);
                notes.clear();
                continue;
            }
            LangNote note;
            note.lyric = currentChar;
            note.g2pId = langMgr->analysis(currentChar);
            notes.append(note);
        }

        if (!notes.isEmpty())
            result.append(notes);
        return result;
    }

    QList<QList<LangNote>> CleanLyric::splitCustom(const QString &input, const QStringList &splitter) {
        const auto langMgr = LangMgr::ILanguageManager::instance();

        QList<QList<LangNote>> result;
        QList<LangNote> notes;
        int pos = 0;
        while (pos < input.length()) {
            const int start = pos;
            while (pos < input.length() && !splitter.contains(input[pos]) && input[pos] != ' ' &&
                   !containLinebreak(input[pos])) {
                pos++;
            }

            const auto lyric = input.mid(start, pos - start);
            if (!lyric.isEmpty() && !splitter.contains(lyric) && lyric != ' ') {
                LangNote note;
                note.lyric = lyric;
                note.g2pId = langMgr->analysis(lyric);
                notes.append(note);
            }

            if (containLinebreak(lyric)) {
                if (!notes.isEmpty())
                    result.append(notes);
                notes.clear();
                continue;
            }
            pos++;
        }

        if (!notes.isEmpty())
            result.append(notes);
        return result;
    }
} // namespace FillLyric
