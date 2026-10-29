#include "sead/seadStream.h"
#include "sead/seadBinaryStreamFormat.h"
#include "sead/seadTextStreamFormat.h"

namespace sead {
// 0x0097F87C
BinaryStreamFormat sead::Stream::s_BinaryStreamFormat;
// 0x00AF50BC
TextStreamFormat sead::Stream::s_TextStreamFormat;

// constructs the formats
// 0x007910CC (static initializer)

// 0x00540C08 (name is ours)
void sead::Stream::writeDecorationText(const SafeStringBase<char>& text)
{
    mFormat->writeDecorationText(mSrc, text);
}

} // namespace sead
