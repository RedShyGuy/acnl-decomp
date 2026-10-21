#pragma once

#include "decomp.h"

namespace nw {
namespace font {
// Reads the characters of a string in the encoding of a font (Font::GetCharStrmReader). Member
// names are ours.
class CharStrmReader
{
public:
    typedef u16 (CharStrmReader::*ReadNextCharFunc)();

    u16 ReadNextCharUTF8(); // 0x004D8934 | nintendogs:bytes [tier A]
    u16 ReadNextCharUTF16(); // 0x004D89A0 | nintendogs:bytes [tier A]
    u16 ReadNextCharCP1252(); // 0x004D89B4 | nintendogs:bytes [tier A]
    u16 ReadNextCharSJIS(); // 0x004D88FC | nintendogs:bytes [tier A]

    const void* m_pCharStrm;     // 0x0
    ReadNextCharFunc m_ReadFunc; // 0x4
};
ASSERT_SIZE(CharStrmReader, 0xC);
} // namespace font
} // namespace nw
