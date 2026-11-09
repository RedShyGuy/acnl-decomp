#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"

namespace script {
// finds the n-th character of a text (member names are ours)
// RTTI N6script10DetectCodeE @ 0x008D36DC
// vtable 0x009095CC (vptr 0x009095D4), offset_to_top 0, 8 entries
class DetectCode : public ::script::MsgEngine
{
public:
    DetectCode() : m_Target(0), m_Count(0), m_pFound(NULL), m_IsFound(0) {}
    virtual ~DetectCode(); // 0x005D5CEC slot 0x00
    // 0x005D5CDC slot 0x04 (deleting dtor)
    virtual bool IsContinue(); // 0x005D5CC8 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005D5C98 slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005D5CC4 slot 0x18

    // (inline) the index-th character from the start
    const char16* Detect(const IWord* word, s32 index)
    {
        m_Target = index;
        m_Count = 0;
        m_pFound = NULL;
        m_IsFound = 0;
        const char16* text = word->GetWord();
        if (text != NULL) {
            Run(text);
        }
        return m_pFound;
    }
    // the index-th character from the end (NULL if the text is shorter)
    const char16* DetectFromEnd(const IWord* word, s32 index); // 0x005D5B98

    s32 m_Target;             // 0x60
    s32 m_Count;              // 0x64
    const char16* m_pFound;   // 0x68
    u8 m_IsFound;             // 0x6C
};
ASSERT_SIZE(DetectCode, 0x70);
} // namespace script
