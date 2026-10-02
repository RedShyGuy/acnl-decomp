#pragma once

#include "decomp.h"

namespace script {
// Instantiations found in the binary:
//   script::WordFix<100u>  typeinfo 0x008D3B18  vtable 0x0090A9AC
//   script::WordFix<109u>  typeinfo 0x008D3B24  vtable 0x0090A9E4
//   script::WordFix<10u>  typeinfo 0x008D3B30  vtable 0x0090AA1C
//   script::WordFix<111u>  typeinfo 0x008D3B3C  vtable 0x0090AA54
//   script::WordFix<11u>  typeinfo 0x008D3B48  vtable 0x0090AA8C
//   script::WordFix<120u>  typeinfo 0x008D3B54  vtable 0x0090AAC4
//   script::WordFix<128u>  typeinfo 0x008D3B60  vtable 0x0090AAFC
//   script::WordFix<12u>  typeinfo 0x008D3B6C  vtable 0x0090AB34
//   script::WordFix<13u>  typeinfo 0x008D3B78  vtable 0x0090AB6C
//   script::WordFix<14u>  typeinfo 0x008D3B84  vtable 0x0090ABA4
//   script::WordFix<1536u>  typeinfo 0x008D3B90  vtable 0x0090ABDC
//   script::WordFix<15u>  typeinfo 0x008D3B9C  vtable 0x0090AC14
//   script::WordFix<16u>  typeinfo 0x008D3BA8  vtable 0x0090AC4C
//   script::WordFix<17u>  typeinfo 0x008D3BB4  vtable 0x0090AC84
//   script::WordFix<188u>  typeinfo 0x008D3BC0  vtable 0x0090ACBC
//   script::WordFix<18u>  typeinfo 0x008D3BCC  vtable 0x0090ACF4
//   script::WordFix<193u>  typeinfo 0x008D3BD8  vtable 0x0090AD2C
//   script::WordFix<197u>  typeinfo 0x008D3BE4  vtable 0x0090AD64
//   script::WordFix<19u>  typeinfo 0x008D3BF0  vtable 0x0090AD9C
//   script::WordFix<1u>  typeinfo 0x008D3BFC  vtable 0x0090ADD4
//   script::WordFix<20u>  typeinfo 0x008D3C08  vtable 0x0090AE0C
//   script::WordFix<21u>  typeinfo 0x008D3C14  vtable 0x0090AE44
//   script::WordFix<22u>  typeinfo 0x008D3C20  vtable 0x0090AE7C
//   script::WordFix<24u>  typeinfo 0x008D3C2C  vtable 0x0090AEB4
//   script::WordFix<256u>  typeinfo 0x008D3C38  vtable 0x0090AEEC
//   script::WordFix<25u>  typeinfo 0x008D3C44  vtable 0x0090AF24
//   script::WordFix<29u>  typeinfo 0x008D3C50  vtable 0x0090AF5C
//   script::WordFix<2u>  typeinfo 0x008D3C5C  vtable 0x0090AF94
//   script::WordFix<30u>  typeinfo 0x008D3C68  vtable 0x0090AFCC
//   script::WordFix<32u>  typeinfo 0x008D3C74  vtable 0x0090B004
//   script::WordFix<33u>  typeinfo 0x008D3C80  vtable 0x0090B03C
//   script::WordFix<350u>  typeinfo 0x008D3C8C  vtable 0x0090B074
//   script::WordFix<36u>  typeinfo 0x008D3C98  vtable 0x0090B0AC
//   script::WordFix<38u>  typeinfo 0x008D3CA4  vtable 0x0090B0E4
//   script::WordFix<400u>  typeinfo 0x008D3CB0  vtable 0x0090B11C
//   script::WordFix<40u>  typeinfo 0x008D3CBC  vtable 0x0090B154
//   script::WordFix<41u>  typeinfo 0x008D3CC8  vtable 0x0090B18C
//   script::WordFix<450u>  typeinfo 0x008D3CD4  vtable 0x0090B1C4
//   script::WordFix<45u>  typeinfo 0x008D3CE0  vtable 0x0090B1FC
//   script::WordFix<4u>  typeinfo 0x008D3CEC  vtable 0x0090B234
//   script::WordFix<50u>  typeinfo 0x008D3CF8  vtable 0x0090B26C
//   script::WordFix<512u>  typeinfo 0x008D3D04  vtable 0x0090B2A4
//   script::WordFix<57u>  typeinfo 0x008D3D10  vtable 0x0090B2DC
//   script::WordFix<60u>  typeinfo 0x008D3D1C  vtable 0x0090B314
//   script::WordFix<64u>  typeinfo 0x008D3D28  vtable 0x0090B34C
//   script::WordFix<65u>  typeinfo 0x008D3D34  vtable 0x0090B384
//   script::WordFix<69u>  typeinfo 0x008D3D40  vtable 0x0090B3BC
//   script::WordFix<70u>  typeinfo 0x008D3D4C  vtable 0x0090B3F4
//   script::WordFix<75u>  typeinfo 0x008D3D58  vtable 0x0090B42C
//   script::WordFix<85u>  typeinfo 0x008D3D64  vtable 0x0090B464
//   script::WordFix<8u>  typeinfo 0x008D3D70  vtable 0x0090B49C
//   script::WordFix<94u>  typeinfo 0x008D3D7C  vtable 0x0090B4D4
//   script::WordFix<9u>  typeinfo 0x008D3D88  vtable 0x0090B50C
template <auto T0>
class WordFix
{
public:
    // TODO: members unknown
};
} // namespace script
