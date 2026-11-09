#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"

namespace script {
// copies a text without its tags (into the words of the save data; member names are ours)
// RTTI N6script18CopyRepelTagEngineE @ 0x008D3A04
// vtable 0x0090A49C (vptr 0x0090A4A4), offset_to_top 0, 8 entries
class CopyRepelTagEngine : public ::script::MsgEngine
{
public:
    CopyRepelTagEngine() : m_pDst(NULL), m_pDstLast(NULL), m_IsFull(0) {}
    virtual ~CopyRepelTagEngine(); // 0x005E9B8C slot 0x00
    // 0x005E9B7C slot 0x04 (deleting dtor)
    virtual bool IsContinue(); // 0x005E9B68 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005E9B40 slot 0x14

    // (inline)
    void Copy(IWord* dst, const IWord* src)
    {
        const char16* text = src->GetWord();
        u32 srcSize = src->GetSize();
        if (text == NULL || srcSize == 0 || dst == src) {
            return;
        }
        char16* buffer = dst->GetWord();
        u32 size = dst->GetSize();
        if (buffer == NULL || size == 0) {
            return;
        }
        dst->Clear();
        m_pDst = buffer;
        m_pDstLast = buffer + size - 1;
        m_IsFull = 0;
        Run(text);
    }

    char16* m_pDst;       // 0x60
    char16* m_pDstLast;   // 0x64
    u8 m_IsFull;          // 0x68
};
ASSERT_SIZE(CopyRepelTagEngine, 0x6C);
} // namespace script
