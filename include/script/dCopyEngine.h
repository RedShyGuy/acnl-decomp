#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"

namespace script {
// copies a text into a word, as far as it fits (member names are ours)
// RTTI N6script10CopyEngineE @ 0x008D36C4
// vtable 0x009095A4 (vptr 0x009095AC), offset_to_top 0, 8 entries
class CopyEngine : public ::script::MsgEngine
{
public:
    CopyEngine() : m_pLimit(NULL), m_pStop(NULL) {}
    virtual ~CopyEngine(); // 0x005D5B88 slot 0x00
    // 0x005D5B78 slot 0x04 (deleting dtor)
    virtual bool IsContinue(); // 0x005D5B58 slot 0x10

    void Copy(IWord* dst, const IWord* src); // 0x005D5A38 (name is ours)

    const char16* m_pLimit;  // 0x60 the last character of the source that fits
    const char16* m_pStop;   // 0x64 where the copy stops
};
ASSERT_SIZE(CopyEngine, 0x68);
} // namespace script
