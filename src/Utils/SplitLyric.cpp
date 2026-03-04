#include "SplitLyric.h"

#include <LangCore/Core/Manager.h>

#include <lyric-tab/LangCommon.h>

namespace FillLyric
{
    QList<QList<LangNote>> CleanLyric::splitAuto(const QString &input, const std::vector<std::string> &priorityG2pIds) {
        QList<QList<LangNote>> result;
        QList<LangNote> notes;
        const auto langMgr = LangCore::Manager::instance();
        const auto res = langMgr->tag({input.toStdString()}, true, true, priorityG2pIds);

        for (const auto &tagger_res : res) {
            if (tagger_res.tag == "linebreak") {
                if (!notes.isEmpty())
                    result.append(notes);
                notes.clear();
                continue;
            }
            auto tempNote = LangNote(tagger_res.lyric.c_str());
            tempNote.language = tagger_res.language.c_str();
            tempNote.g2pId = tagger_res.language.c_str();
            notes.append(tempNote);
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
        const auto langMgr = LangCore::Manager::instance();

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
            const auto taggerRes = langMgr->tag({QString(currentChar).toStdString()}, false, false, {});
            note.g2pId = taggerRes.front().language.c_str();
            note.language = taggerRes.front().language.c_str();
            notes.append(note);
        }

        if (!notes.isEmpty())
            result.append(notes);
        return result;
    }

    QList<QList<LangNote>> CleanLyric::splitCustom(const QString &input, const QStringList &splitter) {
        const auto langMgr = LangCore::Manager::instance();

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
                const auto taggerRes = langMgr->tag({lyric.toStdString()}, false, false, {});
                note.g2pId = taggerRes.front().language.c_str();
                note.language = taggerRes.front().language.c_str();
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
