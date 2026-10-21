#include "nw/font/font_CharStrmReader.h"

namespace nw {
namespace font {
// 0x004D88FC | nintendogs:bytes [tier A]
u16 nw::font::CharStrmReader::ReadNextCharSJIS()
{
    const u8* pStrm = static_cast<const u8*>(m_pCharStrm);
    u16 code = pStrm[0];
    // the lead bytes of two byte characters: 0x81 - 0x9F and 0xE0 - 0xFF
    if (static_cast<u32>(code - 0x81) < 0x1F || code >= 0xE0) {
        code = (code << 8) | pStrm[1];
        m_pCharStrm = pStrm + 2;
    } else {
        m_pCharStrm = pStrm + 1;
    }
    return code;
}

// 0x004D8934 | nintendogs:bytes [tier A]
u16 nw::font::CharStrmReader::ReadNextCharUTF8()
{
    const u8* pStrm = static_cast<const u8*>(m_pCharStrm);
    u16 code;
    if ((pStrm[0] & 0x80) == 0) {
        code = pStrm[0];
        m_pCharStrm = pStrm + 1;
    } else if ((pStrm[0] & 0xE0) == 0xC0) {
        code = ((pStrm[0] & 0x1F) << 6) | (pStrm[1] & 0x3F);
        m_pCharStrm = pStrm + 2;
    } else {
        code = static_cast<u16>((pStrm[0] << 12) | ((pStrm[1] & 0x3F) << 6) | (pStrm[2] & 0x3F));
        m_pCharStrm = pStrm + 3;
    }
    return code;
}

// 0x004D89A0 | nintendogs:bytes [tier A]
u16 nw::font::CharStrmReader::ReadNextCharUTF16()
{
    const u16* pStrm = static_cast<const u16*>(m_pCharStrm);
    u16 code = *pStrm++;
    m_pCharStrm = pStrm;
    return code;
}

// 0x004D89B4 | nintendogs:bytes [tier A]
u16 nw::font::CharStrmReader::ReadNextCharCP1252()
{
    const u8* pStrm = static_cast<const u8*>(m_pCharStrm);
    u16 code = *pStrm++;
    m_pCharStrm = pStrm;
    return code;
}

} // namespace font
} // namespace nw
