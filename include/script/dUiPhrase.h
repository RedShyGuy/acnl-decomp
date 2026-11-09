#pragma once

#include "decomp.h"
#include "script/dPhrase.h"
#include "script/dUiInsert.h"

namespace script {
class UiInsert;

// the phrase of the ui messages (member name is ours)
// RTTI N6script8UiPhraseE @ 0x008D3E14
// vtable 0x0090BA24 (vptr 0x0090BA2C), offset_to_top 0, 2 entries
class UiPhrase : public ::script::Phrase
{
public:
    // (inline in the original)
    // (inline in UiSystem)
    explicit UiPhrase(UiInsert* pInsert);
    virtual ~UiPhrase();

    UiInsert* m_pUiInsert; // 0x8
};

inline UiPhrase::UiPhrase(UiInsert* pInsert) : Phrase(pInsert), m_pUiInsert(pInsert) {}
} // namespace script
