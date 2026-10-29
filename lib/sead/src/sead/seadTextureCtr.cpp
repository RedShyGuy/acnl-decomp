#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadTexture.h"
#include "sead/seadTextureCtr.h"

namespace sead {
// (out of line: TextureCtrGR calls it)
// 0x0053FD6C (name is ours)
DECOMP_NOINLINE sead::TextureCtr::TextureCtr() : Texture()
{
}

// 0x0053FD84 slot 0x08
// 0x0053FD7C (deleting dtor)
sead::TextureCtr::~TextureCtr()
{
    // empty (in the original too)
}

} // namespace sead

// the type information of the classes below Texture
// 0x00829790 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::Texture>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
