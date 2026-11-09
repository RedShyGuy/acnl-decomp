#pragma once

#include "decomp.h"
#include "script/dDialogCompo.h"
#include "script/dMsgEngine.h"

// Writes a page into a word (without the page breaks and the tags 19/x; names are ours).
// RTTI N6script11DialogCompo7PagePutE @ 0x008D37B0
// vtable 0x00909DEC (vptr 0x00909DF4), offset_to_top 0, 8 entries
class script::DialogCompo::PagePut : public ::script::MsgEngine
{
public:
    // (inline)
    PagePut() : m_IsOk(1), m_pWord(NULL), m_pWrite(NULL), m_pLast(NULL), m_IsSuccess(1) {}
    virtual ~PagePut(); // 0x005D9988 slot 0x00
    // 0x005D9978 slot 0x04 (deleting dtor)
    virtual void OnChar(const CharInfo* info); // 0x005D98EC slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005D9924 slot 0x18

    u8 m_IsOk;          // 0x60
    IWord* m_pWord;     // 0x64 the page
    char16* m_pWrite;   // 0x68
    char16* m_pLast;    // 0x6C
    u8 m_IsSuccess;     // 0x70 the text fitted
};
