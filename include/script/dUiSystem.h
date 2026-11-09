#pragma once

#include "decomp.h"
#include "script/dWordPtr.h"

namespace nn {
namespace fnd {
struct DateTimeParameters;
} // namespace fnd
} // namespace nn

namespace script {
class IUiRecept;
class UiInsert;
class UiPhrase;
class UiSeq;

// The writer of the texts of the user interface (UiMgr +0x10; no RTTI, the name is ours, after TalkSystem): a string of
// the string lists with the inserted words of the recept (names are ours).
class UiSystem
{
public:
    static const u32 TEXT_SIZE = 1408;

    UiSystem(); // 0x005FD430
    // (inline)
    ~UiSystem();

    UiInsert* m_pInsert;                  // 0x000
    UiPhrase* m_pPhrase;                  // 0x004
    UiSeq* m_pSeq;                        // 0x008
    nn::fnd::DateTimeParameters* m_pDate; // 0x00C the date of the texts
    u8 m_Unknown0x10;                     // 0x010 (1)
    IUiRecept* m_pRecept;                 // 0x014
    WordPtr m_Text;                       // 0x018 the string of the list (m_TextBuffer)
    char16 m_TextBuffer[TEXT_SIZE];       // 0x024
};
ASSERT_SIZE(UiSystem, 0xB24);
} // namespace script
