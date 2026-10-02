#pragma once

#include "decomp.h"

namespace nw {
namespace font {
class CharStrmReader
{
public:
    void ReadNextCharSJIS(); // 0x004D88FC | nintendogs:bytes [tier A]
    void ReadNextCharUTF8(); // 0x004D8934 | nintendogs:bytes [tier A]
    void ReadNextCharUTF16(); // 0x004D89A0 | nintendogs:bytes [tier A]
    void ReadNextCharCP1252(); // 0x004D89B4 | nintendogs:bytes [tier A]
};
} // namespace font
} // namespace nw
