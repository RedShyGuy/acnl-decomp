#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"

namespace script {
// copies a text with the readings of the ruby tags instead of their kanji (member names are ours)
// RTTI N6script22CopyExcludeKanjiEngineE @ 0x008D3A34
// vtable 0x0090A4EC (vptr 0x0090A4F4), offset_to_top 0, 8 entries
class CopyExcludeKanjiEngine : public ::script::MsgEngine
{
public:
    CopyExcludeKanjiEngine(); // 0x005EA2C8 (name after the RTTI)
    virtual ~CopyExcludeKanjiEngine(); // 0x005EA300 slot 0x00
    // 0x005EA2F0 slot 0x04 (deleting dtor)
    virtual bool IsContinue(); // 0x005EA2B4 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005EA0D8 slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005EA100 slot 0x18

    // a ruby tag (group 0): copies the reading and skips the base text
    void OnRuby(const Tag* tag); // 0x005EA140
    void Copy(IWord* dst, const IWord* src); // 0x005EA1E4

    char16* m_pDst;       // 0x60
    char16* m_pDstLast;   // 0x64
    u8 m_IsFull;          // 0x68
};
ASSERT_SIZE(CopyExcludeKanjiEngine, 0x6C);
} // namespace script
