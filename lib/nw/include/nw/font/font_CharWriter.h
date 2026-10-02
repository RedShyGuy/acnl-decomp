#pragma once

#include "decomp.h"

namespace nw {
namespace font {
class CharWriter
{
public:
    void PrintGlyph(float, const nw::font::Glyph&); // 0x004D61CC | nintendogs:bytes-fuzzy [tier A]
    void StartPrint(); // 0x004D64B4 | libgarden [tier A]
    void SetFontSize(float, float); // 0x004D64D0 | libgarden [tier A]
    void UseCommandBuffer(unsigned long*, nw::font::RectDrawer*); // 0x004D652C | libgarden [tier A]
    void UpdateVertexColors(); // 0x004D6738 | libgarden [tier A]
    void InitDispStringBuffer(void*, unsigned long); // 0x004D6790 | libgarden [tier A]
    void GetDispStringBufferSize(unsigned long); // 0x004D67A0 | libgarden [tier A]
    void Print(unsigned short); // 0x004D6AA8 | nintendogs:bytes [tier A]
    CharWriter(); // 0x004D6E38 | nintendogs:callgraph [tier A]
    void GetFontWidth() const; // 0x00744E34 | nintendogs:bytes [tier A]
    void GetFontAscent() const; // 0x00744E60 | nintendogs:bytes [tier A]
    void GetFontHeight() const; // 0x00744E8C | nintendogs:bytes [tier A]
};
} // namespace font
} // namespace nw
