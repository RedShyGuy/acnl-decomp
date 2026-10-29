#pragma once

#include "decomp.h"
#include "sead/seadTextureCtrGR.h"

namespace sead {
namespace ptcl {
// RTTI N4sead4ptcl11TextureDataE @ 0x008D204C
// vtable 0x00906AE8 (vptr 0x00906AF0), offset_to_top 0, 2 entries
// A texture the editor sent (the class is from the RTTI, the member names are ours).
class TextureData
{
public:
    TextureData(); // 0x00555AA4 | mk7dlp:bytes [tier B]
    virtual ~TextureData(); // 0x00555B0C slot 0x00
    // 0x00555ACC slot 0x04 (deleting dtor)

    u8 _unk_0x04[0x10];
    u8* mImage;            // 0x14
    u8 _unk_0x18[0x8];
    bool mIsReady;         // 0x20
    TextureCtrGR mTexture; // 0x24
};
ASSERT_OFFSET(TextureData, mImage, 0x14);
ASSERT_OFFSET(TextureData, mTexture, 0x24);
ASSERT_SIZE(TextureData, 0x38);
} // namespace ptcl
} // namespace sead
