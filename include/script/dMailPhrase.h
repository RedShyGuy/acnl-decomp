#pragma once

#include "decomp.h"
#include "script/dPhrase.h"
#include "script/dMailInsert.h"

namespace script {
class MailInsert;

// the phrase of the mail messages (member name is ours)
// RTTI N6script10MailPhraseE @ 0x008D3720
// vtable 0x00909950 (vptr 0x00909958), offset_to_top 0, 2 entries
class MailPhrase : public ::script::Phrase
{
public:
    // (inline in the original)
    // (inline in MailSystem)
    explicit MailPhrase(MailInsert* pInsert);
    virtual ~MailPhrase();

    MailInsert* m_pMailInsert; // 0x8
};

inline MailPhrase::MailPhrase(MailInsert* pInsert) : Phrase(pInsert), m_pMailInsert(pInsert) {}
} // namespace script
