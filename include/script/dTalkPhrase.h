#pragma once

#include "decomp.h"
#include "script/dPhrase.h"
#include "script/dTalkInsert.h"

namespace script {
class TalkInsert;

// the phrase of the talk messages (member name is ours)
// RTTI N6script10TalkPhraseE @ 0x008D375C
// vtable 0x00909CB0 (vptr 0x00909CB8), offset_to_top 0, 2 entries
class TalkPhrase : public ::script::Phrase
{
public:
    // (inline)
    explicit TalkPhrase(TalkInsert* pInsert) : Phrase(pInsert), m_pTalkInsert(pInsert) {}
    virtual ~TalkPhrase();

    TalkInsert* m_pTalkInsert; // 0x8
};
} // namespace script
