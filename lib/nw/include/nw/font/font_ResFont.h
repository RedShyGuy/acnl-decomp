#pragma once

#include "decomp.h"
#include "nw/font/font_ResFontBase.h"

namespace nw {
namespace font {
// RTTI N2nw4font7ResFontE @ 0x008D0C18
// vtable 0x009037C8 (vptr 0x009037D0), offset_to_top 0, 27 entries
//
// A font resource in memory (BCFNT). The resource is relocated in place: its signature becomes
// 'CFNU' and the offsets become pointers.
class ResFont : public ::nw::font::ResFontBase
{
public:
    // the signatures of a font resource before and after the relocation
    static const u32 SIGNATURE = 0x544E4643;            // 'CFNT'
    static const u32 SIGNATURE_UNPACKED = 0x554E4643;   // 'CFNU'
    // the supported version and the blocks it needs
    static const u32 VERSION = 0x03000000;
    static const u16 MIN_BLOCKS = 2;
    // the block kinds (names are ours)
    static const u32 BLOCK_FINF = 0x464E4946; // 'FINF'
    static const u32 BLOCK_CGLP = 0x504C4743; // 'CGLP'
    static const u32 BLOCK_TGLP = 0x504C4754; // 'TGLP'
    static const u32 BLOCK_CWDH = 0x48445743; // 'CWDH'
    static const u32 BLOCK_CMAP = 0x50414D43; // 'CMAP'
    static const u32 BLOCK_GLGR = 0x52474C47; // 'GLGR'

    ResFont(); // 0x004D95D8 (name is ours)
    virtual ~ResFont(); // 0x001386B0 slot 0x00
    // 0x004D95F0 slot 0x04 (deleting dtor)

    // false: there is a resource already, or the resource is not valid
    bool SetResource(void* pBfnt); // 0x004D9368 (name is ours)
    // the resource (NULL: none)
    void* RemoveResource(); // 0x0013C4B8 (name is ours)
    // the old draw buffer
    void* SetDrawBuffer(void* pBuffer); // 0x0013C52C | nintendogs:bytes [tier A]
    // the size of the draw buffer of the resource (0: not valid)
    static u32 GetDrawBufferSize(const void* pBfnt); // 0x004D9540 | nintendogs:bytes [tier A]
};
} // namespace font
} // namespace nw
