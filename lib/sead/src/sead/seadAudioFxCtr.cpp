#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAudioFx.h"
#include "sead/seadAudioFxCtr.h"

namespace sead {
// 0x00141B98 slot 0x08 | virtual slot, introduced by sead::AudioFx
void sead::AudioFxCtr::vf_0x08()
{
}

// 0x0053E214 slot 0x0C | nintendogs:bytes
void sead::AudioFxCtr::vf_0x0C()
{
}

// 0x00132CEC | nintendogs:bytes [tier B]
sead::AudioFxCtr::AudioFxCtr()
{
}

} // namespace sead

// the type information of the classes below AudioFx
// 0x0082971C (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::AudioFx>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
