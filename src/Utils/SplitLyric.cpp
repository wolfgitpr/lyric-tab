#include "SplitLyric.h"

#include "TextSplitter.h"
#include "TextTagger.h"

#include <lyric-tab/LangCommon.h>

namespace FillLyric
{
    QList<QList<LangNote>> LyricSplitter::splitAuto(const QString &input,
                                                     const std::vector<std::string> &priorityG2pIds) {
        QList<QList<LangNote>> result;
        QList<LangNote> notes;

        const auto splitRes = TextSplitter::split(input.toStdString());
        const auto tagRes = TextTagger::tag(splitRes, true, priorityG2pIds);

        for (const auto &tagger_res : tagRes) {
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

    QList<QList<LangNote>> LyricSplitter::splitByChar(const QString &input) {
        // Collect all non-space, non-linebreak characters and their positions,
        // then batch-tag them in a single call.
        struct CharInfo {
            int lineIdx;
            QChar ch;
        };

        QList<QList<LangNote>> result;
        std::vector<CharInfo> chars;
        std::vector<std::string> taggerInput;
        int currentLine = 0;

        // First pass: collect characters and track line breaks
        // We need to know the line structure to build the result
        struct LineBreakPos {
            int charIndex; // index in chars vector where a new line starts
        };
        std::vector<int> lineBreaks; // indices in chars where a linebreak occurs

        for (int i = 0; i < input.length(); i++) {
            const QChar &c = input[i];
            if (c == ' ')
                continue;
            if (containLinebreak(c)) {
                lineBreaks.push_back(static_cast<int>(chars.size()));
                continue;
            }
            chars.push_back({currentLine, c});
            taggerInput.push_back(QString(c).toStdString());
        }

        if (taggerInput.empty())
            return result;

        // Single batch tag call
        const auto taggerRes = TextTagger::tag(taggerInput, false, {});

        // Build result, inserting line breaks at the right positions
        QList<LangNote> notes;
        size_t lbIdx = 0;
        for (size_t i = 0; i < chars.size(); i++) {
            // Check if any linebreaks should be inserted before this character
            while (lbIdx < lineBreaks.size() && lineBreaks[lbIdx] == static_cast<int>(i)) {
                if (!notes.isEmpty())
                    result.append(notes);
                notes.clear();
                lbIdx++;
            }

            LangNote note;
            note.lyric = chars[i].ch;
            if (i < taggerRes.size()) {
                note.g2pId = taggerRes[i].language.c_str();
                note.language = taggerRes[i].language.c_str();
            }
            notes.append(note);
        }
        // Handle trailing linebreaks
        while (lbIdx < lineBreaks.size()) {
            if (!notes.isEmpty())
                result.append(notes);
            notes.clear();
            lbIdx++;
        }

        if (!notes.isEmpty())
            result.append(notes);
        return result;
    }

    QList<QList<LangNote>> LyricSplitter::splitCustom(const QString &input, const QStringList &splitter) {
        // First pass: collect all words
        struct WordInfo {
            QString lyric;
            bool isLineBreak;
        };
        std::vector<WordInfo> words;
        std::vector<std::string> taggerInput;
        std::vector<size_t> taggerToWordIndex; // maps tagger result index -> words index

        int pos = 0;
        while (pos < input.length()) {
            // Check for linebreak
            if (containLinebreak(input[pos])) {
                words.push_back({QString(), true});
                pos++;
                continue;
            }

            // Skip spaces and splitters
            if (input[pos] == ' ' || splitter.contains(input[pos])) {
                pos++;
                continue;
            }

            const int start = pos;
            while (pos < input.length() && !splitter.contains(input[pos]) && input[pos] != ' ' &&
                   !containLinebreak(input[pos])) {
                pos++;
            }

            const auto lyric = input.mid(start, pos - start);
            if (!lyric.isEmpty()) {
                taggerToWordIndex.push_back(words.size());
                words.push_back({lyric, false});
                taggerInput.push_back(lyric.toStdString());
            }
        }

        if (taggerInput.empty()) {
            // Still handle linebreaks
            QList<QList<LangNote>> result;
            return result;
        }

        // Single batch tag call
        const auto taggerRes = TextTagger::tag(taggerInput, false, {});

        // Build result
        QList<QList<LangNote>> result;
        QList<LangNote> notes;
        size_t taggerIdx = 0;

        for (size_t i = 0; i < words.size(); i++) {
            if (words[i].isLineBreak) {
                if (!notes.isEmpty())
                    result.append(notes);
                notes.clear();
                continue;
            }

            LangNote note;
            note.lyric = words[i].lyric;
            if (taggerIdx < taggerRes.size()) {
                note.g2pId = taggerRes[taggerIdx].language.c_str();
                note.language = taggerRes[taggerIdx].language.c_str();
                taggerIdx++;
            }
            notes.append(note);
        }

        if (!notes.isEmpty())
            result.append(notes);
        return result;
    }
}
