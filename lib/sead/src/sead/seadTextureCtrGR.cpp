#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadTextureCtr.h"
#include "sead/seadTextureCtrGR.h"

namespace sead {
// 0x0053FD80 slot 0x08
// 0x00541308 (deleting dtor)
sead::TextureCtrGR::~TextureCtrGR()
{
    // (only the bases are destroyed)
}

// 0x005412D4 | nintendogs:bytes [tier A]
sead::TextureCtrGR::TextureCtrGR() : TextureCtr(), mWidth(1), mHeight(1), field_0x0C(0), field_0x10(0), field_0x11(0)
{
}

} // namespace sead

// the type information of the classes below TextureCtr
// 0x00828988 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::TextureCtr>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
