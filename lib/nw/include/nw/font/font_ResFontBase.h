#pragma once

#include "decomp.h"
#include "nw/font/font_Font.h"

namespace nw {
namespace font {
// RTTI N2nw4font11ResFontBaseE @ 0x008D0BF4
// vtable 0x009036B4 (vptr 0x009036BC), offset_to_top 0, 27 entries
class ResFontBase : public ::nw::font::Font
{
public:
    ResFontBase(); // ctor candidate(s) 0x004D88B0 (unverified)
    virtual void vf_0x00(); // 0x0013F634 slot 0x00 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x04(); // 0x004D88EC slot 0x04 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x08(); // 0x007452A0 slot 0x08 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x0C(); // 0x007452D4 slot 0x0C | virtual slot, introduced by nw::font::Font
    virtual void vf_0x10(); // 0x007452C8 slot 0x10 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x14(); // 0x00744EF4 slot 0x14 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x18(); // 0x007450D8 slot 0x18 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x1C(); // 0x00745298 slot 0x1C | virtual slot, introduced by nw::font::Font
    virtual void vf_0x20(); // 0x007450F4 slot 0x20 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x24(); // 0x00744F08 slot 0x24 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x28(); // 0x00745264 slot 0x28 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x2C(); // 0x004D8754 slot 0x2C | virtual slot, introduced by nw::font::Font
    virtual void vf_0x30(); // 0x004D8898 slot 0x30 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x34(); // 0x004D8760 slot 0x34 | virtual slot, introduced by nw::font::Font
    virtual void GetCharWidth(unsigned short) const; // 0x00744F24 slot 0x38 | nintendogs:bytes
    virtual void GetCharWidths(unsigned short) const; // 0x00744F50 slot 0x3C | nintendogs:bytes
    virtual void GetGlyph(nw::font::Glyph*, unsigned short) const; // 0x00745114 slot 0x40 | nintendogs:bytes
    virtual void vf_0x44(); // 0x007452AC slot 0x44 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x48(); // 0x007450E8 slot 0x48 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x4C(); // 0x007450C8 slot 0x4C | virtual slot, introduced by nw::font::Font
    virtual void vf_0x50(); // 0x00744F40 slot 0x50 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x54(); // 0x00744F14 slot 0x54 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x58(); // 0x004D8794 slot 0x58 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x5C(); // 0x00745288 slot 0x5C | virtual slot, introduced by nw::font::Font
    virtual void vf_0x60(); // 0x00745278 slot 0x60 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x64(); // 0x00745270 slot 0x64 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x68(); // 0x00745104 slot 0x68 | virtual slot, introduced by nw::font::ResFontBase
    void DeleteTextureNames(); // 0x0013F5DC | nintendogs:bytes [tier A]
    void GenTextureNames(); // 0x001426DC | nintendogs:bytes-fuzzy [tier A]
    void FindGlyphIndex(unsigned short) const; // 0x00744FC8 | nintendogs:bytes [tier A]
    void GetGlyphFromIndex(nw::font::Glyph*, unsigned short) const; // 0x0074514C | nintendogs:bytes [tier A]
};
} // namespace font
} // namespace nw
