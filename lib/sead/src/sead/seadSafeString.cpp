#include "sead/seadSafeStringBase.h"
#include "sead/seadBufferedSafeStringBase.h"
#include "sead/seadFixedSafeStringBase.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadWFixedSafeString.h"
#include "sead/seadFormatFixedSafeString.h"

// The functions of the SafeString classes and the copies of their template members (the name of the
// file is ours).

namespace sead {
// 0x0030F648 (name is ours)
template const char* sead::SafeStringBase<char>::getCString() const;

// 0x008283D8 slot 0x08 (symbols_us: SetNT)
template void sead::SafeStringBase<char>::assureTerminationImpl_() const;
// 0x00828738 slot 0x08
template void sead::SafeStringBase<wchar_t>::assureTerminationImpl_() const;
// (0x00828328 is a second copy, from the object that defines s_NullChar: ARMCC stores the constant 0
// there; GCC emits no copy in seadSafeStringBase.cpp, so it is compared with this one)
// 0x00829C00 slot 0x08
// 0x00828328 (copy)
template void sead::BufferedSafeStringBase<char>::assureTerminationImpl_() const;
// 0x00829C1C slot 0x08
template void sead::BufferedSafeStringBase<wchar_t>::assureTerminationImpl_() const;

// the constructors that are not inline in their callers
// 0x00812000 | nintendogs:callgraph
template <>
sead::FixedSafeString<32>::FixedSafeString()
{
    // (only the bases are constructed)
}
// 0x00135B04 (name is ours)
template <>
sead::FixedSafeString<256>::FixedSafeString()
{
    // (only the bases are constructed)
}
// 0x00812148 (name is ours)
template <>
sead::FixedSafeString<64>::FixedSafeString()
{
    // (only the bases are constructed)
}
// 0x008122EC (name is ours)
template <>
sead::FixedSafeString<96>::FixedSafeString()
{
    // (only the bases are constructed)
}
// 0x00811DCC (name is ours)
template <>
sead::FixedSafeString<17>::FixedSafeString()
{
    // (only the bases are constructed)
}
// 0x00812290 (name is ours)
template <>
sead::FixedSafeString<8>::FixedSafeString()
{
    // (only the bases are constructed)
}
// 0x008121A4 (name is ours)
template <>
sead::FixedSafeString<7>::FixedSafeString(const SafeStringBase<char>& str) : FixedSafeStringBase<char, 7>(str)
{
    // (only the bases are constructed)
}
// 0x00811E28 (name is ours)
template <>
sead::FixedSafeString<20>::FixedSafeString(const SafeStringBase<char>& str) : FixedSafeStringBase<char, 20>(str)
{
    // (only the bases are constructed)
}
// 0x00811F14 (name is ours)
// 0x00545BEC (copy)
template <>
sead::FixedSafeString<32>::FixedSafeString(const SafeStringBase<char>& str) : FixedSafeStringBase<char, 32>(str)
{
    // (only the bases are constructed)
}
// 0x0081205C (name is ours)
template <>
sead::FixedSafeString<64>::FixedSafeString(const SafeStringBase<char>& str) : FixedSafeStringBase<char, 64>(str)
{
    // (only the bases are constructed)
}
// 0x00812B38 (name is ours)
template <>
sead::FormatFixedSafeString<128>::FormatFixedSafeString(const char* fmt, ...) : FixedSafeString<128>()
{
    std::va_list args;
    va_start(args, fmt);
    formatV(fmt, args);
    va_end(args);
}
// 0x00812BB8 (name is ours)
template <>
sead::FormatFixedSafeString<32>::FormatFixedSafeString(const char* fmt, ...) : FixedSafeString<32>()
{
    std::va_list args;
    va_start(args, fmt);
    formatV(fmt, args);
    va_end(args);
}
// 0x00812C38 (name is ours)
template <>
sead::FormatFixedSafeString<64>::FormatFixedSafeString(const char* fmt, ...) : FixedSafeString<64>()
{
    std::va_list args;
    va_start(args, fmt);
    formatV(fmt, args);
    va_end(args);
}
// 0x00812CB8 (name is ours)
template <>
sead::FormatFixedSafeString<6>::FormatFixedSafeString(const char* fmt, ...) : FixedSafeString<6>()
{
    std::va_list args;
    va_start(args, fmt);
    formatV(fmt, args);
    va_end(args);
}
// 0x00812638
template sead::WFixedSafeString<5>::WFixedSafeString();
// 0x00812E50 (name is ours)
template s32 sead::BufferedSafeStringBase<char>::append(char c, s32 num);
// 0x0013DAA4 (name is ours)
template s32 sead::BufferedSafeStringBase<char>::copy(const SafeStringBase<char>& src, s32 length);
// 0x0013DB58 (name is ours)
template sead::SafeStringBase<char> sead::SafeStringBase<char>::getPart(s32 index) const;
// 0x0013DBE4 (name is ours)
template s32 sead::SafeStringBase<char>::findIndex(const SafeStringBase<char>& str) const;

// the destructors of the instances (empty; the Thumb copies are from Thumb code of the game)
// 0x00813040
// 0x0081303C (deleting dtor)
template sead::BufferedSafeStringBase<char>::~BufferedSafeStringBase();
// 0x008132AC
// 0x008132A8 (deleting dtor)
template sead::BufferedSafeStringBase<wchar_t>::~BufferedSafeStringBase();
// 0x00545BE0
// 0x00545BDC (deleting dtor)
template sead::FixedSafeString<2>::~FixedSafeString();
// 0x00545CF4
// 0x00545CF0 (deleting dtor)
template sead::FixedSafeString<4>::~FixedSafeString();
// 0x00545D0C
// 0x00545D08 (deleting dtor)
template sead::FixedSafeString<6>::~FixedSafeString();
// 0x0010C64C (Thumb)
// 0x0010C640 (deleting dtor)
template sead::FixedSafeString<7>::~FixedSafeString();
// 0x00545D1C
// 0x00545D18 (deleting dtor)
template sead::FixedSafeString<8>::~FixedSafeString();
// 0x00545BC8
// 0x00545BC4 (deleting dtor)
template sead::FixedSafeString<16>::~FixedSafeString();
// 0x00545BD0
// 0x00545BCC (deleting dtor)
template sead::FixedSafeString<17>::~FixedSafeString();
// 0x0010C638 (Thumb)
// 0x0010C62C (deleting dtor)
template sead::FixedSafeString<20>::~FixedSafeString();
// 0x00545BE8
// 0x00545BE4 (deleting dtor)
template sead::FixedSafeString<30>::~FixedSafeString();
// 0x00545CDC
// 0x00545CD8 (deleting dtor)
template sead::FixedSafeString<32>::~FixedSafeString();
// 0x00545CE4
// 0x00545CE0 (deleting dtor)
template sead::FixedSafeString<33>::~FixedSafeString();
// 0x00545CEC
// 0x00545CE8 (deleting dtor)
template sead::FixedSafeString<48>::~FixedSafeString();
// 0x00545D04
// 0x00545D00 (deleting dtor)
template sead::FixedSafeString<64>::~FixedSafeString();
// 0x00545D14
// 0x00545D10 (deleting dtor)
template sead::FixedSafeString<80>::~FixedSafeString();
// 0x00545D24
// 0x00545D20 (deleting dtor)
template sead::FixedSafeString<96>::~FixedSafeString();
// 0x00545BC0
// 0x00545BBC (deleting dtor)
template sead::FixedSafeString<128>::~FixedSafeString();
// 0x00545BD8
// 0x00545BD4 (deleting dtor)
template sead::FixedSafeString<256>::~FixedSafeString();
// 0x00545CFC
// 0x00545CF8 (deleting dtor)
template sead::FixedSafeString<512>::~FixedSafeString();
// 0x00545BB8
// 0x00545BB4 (deleting dtor)
template sead::FixedSafeString<1024>::~FixedSafeString();
// 0x00812A78
// 0x00812A74 (deleting dtor)
template sead::FixedSafeStringBase<char, 2>::~FixedSafeStringBase();
// 0x00812A8C
// 0x00812A88 (deleting dtor)
template sead::FixedSafeStringBase<char, 4>::~FixedSafeStringBase();
// 0x00812AA4
// 0x00812AA0 (deleting dtor)
template sead::FixedSafeStringBase<char, 6>::~FixedSafeStringBase();
// 0x00300654
// 0x00812AA8 (deleting dtor)
template sead::FixedSafeStringBase<char, 7>::~FixedSafeStringBase();
// 0x00812AB0
// 0x00812AAC (deleting dtor)
template sead::FixedSafeStringBase<char, 8>::~FixedSafeStringBase();
// 0x00812A64
// 0x00812A60 (deleting dtor)
template sead::FixedSafeStringBase<char, 17>::~FixedSafeStringBase();
// 0x00307C0C
// 0x00812A68 (deleting dtor)
template sead::FixedSafeStringBase<char, 20>::~FixedSafeStringBase();
// 0x0030F668
// 0x00812A7C (deleting dtor)
template sead::FixedSafeStringBase<char, 32>::~FixedSafeStringBase();
// 0x00812A84
// 0x00812A80 (deleting dtor)
template sead::FixedSafeStringBase<char, 48>::~FixedSafeStringBase();
// 0x00812A9C
// 0x00812A98 (deleting dtor)
template sead::FixedSafeStringBase<char, 64>::~FixedSafeStringBase();
// 0x00812AB8
// 0x00812AB4 (deleting dtor)
template sead::FixedSafeStringBase<char, 96>::~FixedSafeStringBase();
// 0x00812A5C
// 0x00812A58 (deleting dtor)
template sead::FixedSafeStringBase<char, 128>::~FixedSafeStringBase();
// 0x00812A70
// 0x00812A6C (deleting dtor)
template sead::FixedSafeStringBase<char, 256>::~FixedSafeStringBase();
// 0x00812A94
// 0x00812A90 (deleting dtor)
template sead::FixedSafeStringBase<char, 512>::~FixedSafeStringBase();
// 0x00812A54
// 0x00812A50 (deleting dtor)
template sead::FixedSafeStringBase<char, 1024>::~FixedSafeStringBase();
// 0x00812AF8
// 0x00812AF4 (deleting dtor)
template sead::FixedSafeStringBase<wchar_t, 5>::~FixedSafeStringBase();
// 0x00812B10
// 0x00812B0C (deleting dtor)
template sead::FixedSafeStringBase<wchar_t, 6>::~FixedSafeStringBase();
// 0x00812AC0
// 0x00812ABC (deleting dtor)
template sead::FixedSafeStringBase<wchar_t, 18>::~FixedSafeStringBase();
// 0x00812AD8
// 0x00812AD4 (deleting dtor)
template sead::FixedSafeStringBase<wchar_t, 21>::~FixedSafeStringBase();
// 0x00812AE0
// 0x00812ADC (deleting dtor)
template sead::FixedSafeStringBase<wchar_t, 33>::~FixedSafeStringBase();
// 0x00812AE8
// 0x00812AE4 (deleting dtor)
template sead::FixedSafeStringBase<wchar_t, 36>::~FixedSafeStringBase();
// 0x00812AF0
// 0x00812AEC (deleting dtor)
template sead::FixedSafeStringBase<wchar_t, 44>::~FixedSafeStringBase();
// 0x00812B00
// 0x00812AFC (deleting dtor)
template sead::FixedSafeStringBase<wchar_t, 64>::~FixedSafeStringBase();
// 0x00812B08
// 0x00812B04 (deleting dtor)
template sead::FixedSafeStringBase<wchar_t, 65>::~FixedSafeStringBase();
// 0x00812AC8
// 0x00812AC4 (deleting dtor)
template sead::FixedSafeStringBase<wchar_t, 197>::~FixedSafeStringBase();
// 0x00812AD0
// 0x00812ACC (deleting dtor)
template sead::FixedSafeStringBase<wchar_t, 201>::~FixedSafeStringBase();
// 0x0054B470
// 0x0054B46C (deleting dtor)
template sead::FormatFixedSafeString<6>::~FormatFixedSafeString();
// 0x0054B460
// 0x0054B45C (deleting dtor)
template sead::FormatFixedSafeString<32>::~FormatFixedSafeString();
// 0x0054B468
// 0x0054B464 (deleting dtor)
template sead::FormatFixedSafeString<64>::~FormatFixedSafeString();
// 0x0054B458
// 0x0054B454 (deleting dtor)
template sead::FormatFixedSafeString<128>::~FormatFixedSafeString();
// 0x002F6DA0
// 0x00811DA0 (deleting dtor)
template sead::SafeStringBase<char>::~SafeStringBase();
// 0x00811D9C
// 0x00811D98 (deleting dtor)
template sead::SafeStringBase<char>::iterator::~iterator();
// 0x00811DC8
// 0x00811DC4 (deleting dtor)
template sead::SafeStringBase<wchar_t>::~SafeStringBase();
// 0x00811DB4
// 0x00811DA4 (deleting dtor)
template sead::SafeStringBase<wchar_t>::token_iterator::~token_iterator();
// 0x00548B34
// 0x00548B30 (deleting dtor)
template sead::WFixedSafeString<5>::~WFixedSafeString();
// 0x00548B4C
// 0x00548B48 (deleting dtor)
template sead::WFixedSafeString<7>::~WFixedSafeString();
// 0x00548B54
// 0x00548B50 (deleting dtor)
template sead::WFixedSafeString<9>::~WFixedSafeString();
// 0x00548AD4
// 0x00548AD0 (deleting dtor)
template sead::WFixedSafeString<13>::~WFixedSafeString();
// 0x00548ADC
// 0x00548AD8 (deleting dtor)
template sead::WFixedSafeString<18>::~WFixedSafeString();
// 0x00548AF4
// 0x00548AF0 (deleting dtor)
template sead::WFixedSafeString<21>::~WFixedSafeString();
// 0x00548B04
// 0x00548B00 (deleting dtor)
template sead::WFixedSafeString<32>::~WFixedSafeString();
// 0x00548B0C
// 0x00548B08 (deleting dtor)
template sead::WFixedSafeString<33>::~WFixedSafeString();
// 0x00548B14
// 0x00548B10 (deleting dtor)
template sead::WFixedSafeString<36>::~WFixedSafeString();
// 0x00548B1C
// 0x00548B18 (deleting dtor)
template sead::WFixedSafeString<44>::~WFixedSafeString();
// 0x00548B24
// 0x00548B20 (deleting dtor)
template sead::WFixedSafeString<50>::~WFixedSafeString();
// 0x00548B2C
// 0x00548B28 (deleting dtor)
template sead::WFixedSafeString<53>::~WFixedSafeString();
// 0x00548B3C
// 0x00548B38 (deleting dtor)
template sead::WFixedSafeString<64>::~WFixedSafeString();
// 0x00548B44
// 0x00548B40 (deleting dtor)
template sead::WFixedSafeString<65>::~WFixedSafeString();
// 0x00548AE4
// 0x00548AE0 (deleting dtor)
template sead::WFixedSafeString<197>::~WFixedSafeString();
// 0x00548AEC
// 0x00548AE8 (deleting dtor)
template sead::WFixedSafeString<201>::~WFixedSafeString();
// 0x00548AFC
// 0x00548AF8 (deleting dtor)
template sead::WFixedSafeString<256>::~WFixedSafeString();
} // namespace sead
