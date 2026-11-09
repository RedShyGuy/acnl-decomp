#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"

namespace script {
// copies a text line by line into a word (member names are ours)
// RTTI N6script10LineTracerE @ 0x008D36FC
// vtable 0x00909620 (vptr 0x00909628), offset_to_top 0, 8 entries
class LineTracer : public ::script::MsgEngine
{
public:
    LineTracer(); // 0x005D62E0 (name after the RTTI)
    virtual ~LineTracer(); // 0x005D631C slot 0x00
    // 0x005D630C slot 0x04 (deleting dtor)
    virtual void End(); // 0x005D6204 slot 0x0C
    virtual bool IsContinue(); // 0x005D6244 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005D61B4 slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005D6208 slot 0x18

    // the text to read
    void SetSource(const IWord* word); // 0x005D61E4
    // the next line into dst; whether a new line ended it
    bool ReadLine(IWord* dst); // 0x005D6258

    const char16* m_pLine;  // 0x60 the start of the next line
    char16* m_pDst;         // 0x64
    char16* m_pDstLast;     // 0x68
    bool m_IsReturn;        // 0x6C
};
ASSERT_SIZE(LineTracer, 0x70);
} // namespace script
