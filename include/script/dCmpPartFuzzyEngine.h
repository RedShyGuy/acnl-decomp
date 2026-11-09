#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"
#include "script/dCmpFuzzyEngine.h"

namespace script {
// finds a word in a text (compared with IsFuzzyEqual, tags exactly): at every position of the text EngineA walks the
// rest and EngineB the word (member names are ours)
// RTTI N6script18CmpPartFuzzyEngineE @ 0x008D39EC
// vtable 0x0090A474 (vptr 0x0090A47C), offset_to_top 0, 8 entries
class CmpPartFuzzyEngine : public ::script::MsgEngine
{
public:
    // RTTI N6script18CmpPartFuzzyEngine7EngineBE @ 0x008D39D4
    // vtable 0x0090A44C (vptr 0x0090A454), offset_to_top 0, 8 entries
    class EngineB : public ::script::MsgEngine
    {
    public:
        EngineB()
            : m_pInfo(NULL), m_pTag(NULL), m_IsContinue(false), m_IsEnd(false), m_IsMatch(false), m_IsEqual(false),
              m_IsFound(false)
        {
        }
        virtual ~EngineB(); // 0x005E9938 slot 0x00
        // 0x005E9928 slot 0x04 (deleting dtor)
        virtual void End(); // 0x005E98CC slot 0x0C
        virtual bool IsContinue(); // 0x005E9920 slot 0x10
        virtual void OnChar(const CharInfo* info); // 0x005E97CC slot 0x14
        virtual void OnTag(const Tag* tag); // 0x005E98D8 slot 0x18

        const CharInfo* m_pInfo;  // 0x60 the character of the text
        const Tag* m_pTag;        // 0x64 the tag of the text
        bool m_IsContinue;        // 0x68
        bool m_IsEnd;             // 0x69 the whole word was compared
        bool m_IsMatch;           // 0x6A
        u8 _pad0x6B;
        bool m_IsEqual;           // 0x6C (EngineA continues while it is set)
        bool m_IsFound;           // 0x6D
    };
    ASSERT_OFFSET(EngineB, m_IsEqual, 0x6C);

    // RTTI N6script18CmpPartFuzzyEngine7EngineAE @ 0x008D39BC
    // vtable 0x0090A424 (vptr 0x0090A42C), offset_to_top 0, 8 entries
    class EngineA : public ::script::MsgEngine
    {
    public:
        EngineA() {}
        virtual ~EngineA(); // 0x005E97B8 slot 0x00
        // 0x005E97A4 slot 0x04 (deleting dtor)
        virtual bool IsContinue(); // 0x005E979C slot 0x10
        virtual void OnChar(const CharInfo* info); // 0x005E96F4 slot 0x14
        virtual void OnTag(const Tag* tag); // 0x005E9748 slot 0x18

        EngineB m_B;  // 0x60
    };

    CmpPartFuzzyEngine() : m_IsFound(0), m_pText(NULL), m_Size(0), m_pWord(NULL) {}
    virtual ~CmpPartFuzzyEngine(); // 0x005E9B30 slot 0x00
    // 0x005E9B20 slot 0x04 (deleting dtor)
    // compares the word with the rest of the text at the current position
    virtual bool IsContinue(); // 0x005E9948 slot 0x10

    u8 m_IsFound;           // 0x60
    const char16* m_pText;  // 0x64
    u32 m_Size;             // 0x68
    const IWord* m_pWord;   // 0x6C
};
ASSERT_SIZE(CmpPartFuzzyEngine, 0x70);

// (inline) whether word is text (isPart: is in text), compared with IsFuzzyEqual (name is ours)
DECOMP_EMIT_COPY inline bool IsFuzzyMatch(const IWord* text, const IWord* word, bool isPart) // 0x0075C5B8
{
    if (isPart) {
        CmpPartFuzzyEngine engine;
        if (text == word) {
            engine.m_IsFound = 1;
        } else {
            const char16* textA = text->GetWord();
            u32 size = text->GetSize();
            if (textA != NULL && size != 0) {
                engine.m_pText = textA;
                engine.m_Size = size;
                engine.m_pWord = word;
                engine.Run(textA);
            }
        }
        return engine.m_IsFound;
    }
    CmpFuzzyEngine engine;
    if (text == word) {
        engine.m_B.m_IsEqual = true;
    } else {
        const char16* textA = text->GetWord();
        u32 sizeA = text->GetSize();
        const char16* textB = word->GetWord();
        u32 sizeB = word->GetSize();
        if (textA != NULL && sizeA != 0 && textB != NULL && sizeB != 0) {
            engine.SetText(&textA, &MsgEngine::s_pNoStop);
            engine.m_B.SetText(&textB, &MsgEngine::s_pNoStop);
            engine.m_B.m_IsEqual = true;
            engine.SetText2(&MsgEngine::s_pNoStop);
            if (engine.m_B.m_IsEqual) {
                // the second text must end too
                const char16* rest = engine.m_B.m_pCur;
                if (rest == NULL || *rest != 0) {
                    engine.m_B.m_IsEqual = false;
                }
            }
        }
    }
    return engine.m_B.m_IsEqual;
}
} // namespace script
