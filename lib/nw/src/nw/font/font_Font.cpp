#include "nw/font/font_Font.h"

namespace nw {
namespace font {
namespace {
// the readers of the encodings (the index of UTF-16 is the one the wide strings use)
const u32 READ_FUNC_UTF16 = 3;
// 0x008B4728 (name is ours)
const CharStrmReader::ReadNextCharFunc s_ReadFuncs[] = {
    &CharStrmReader::ReadNextCharUTF8,
    &CharStrmReader::ReadNextCharSJIS,
    &CharStrmReader::ReadNextCharCP1252,
    &CharStrmReader::ReadNextCharUTF16,
};
} // namespace

// 0x004D932C (name is ours)
nw::font::Font::Font()
{
    // (only the vtable; a debug log is removed)
}

// 0x0013F638 slot 0x00
// 0x004D9358 (deleting dtor)
nw::font::Font::~Font()
{
    // empty (in the original too)
}

// 0x007452F0 | nintendogs:bytes [tier A]
nw::font::CharStrmReader nw::font::Font::GetCharStrmReader(wchar_t dummy) const
{
    CharStrmReader reader;
    reader.m_pCharStrm = nullptr;
    reader.m_ReadFunc = nullptr;
    if (GetCharacterCode() == CHARACTER_CODE_UTF16) {
        reader.m_ReadFunc = s_ReadFuncs[READ_FUNC_UTF16];
    }
    return reader;
}

} // namespace font
} // namespace nw
