#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAudioResourceLoader.h"
#include "sead/seadAudioResourceLoaderCtr.h"

namespace sead {
// ctor address unknown
sead::AudioResourceLoaderCtr::AudioResourceLoaderCtr()
{
}

// 0x0013F6CC slot 0x08 | virtual slot, introduced by sead::AudioResourceLoaderCtr
void sead::AudioResourceLoaderCtr::vf_0x08()
{
}

// 0x0054B96C slot 0x0C | virtual slot, introduced by sead::AudioResourceLoaderCtr
void sead::AudioResourceLoaderCtr::vf_0x0C()
{
}

// 0x0054B744 slot 0x10 | virtual slot, introduced by sead::AudioResourceLoaderCtr
void sead::AudioResourceLoaderCtr::vf_0x10()
{
}

// 0x0054B84C slot 0x14 | virtual slot, introduced by sead::AudioResourceLoaderCtr
void sead::AudioResourceLoaderCtr::vf_0x14()
{
}

// 0x0054B968 slot 0x18 | virtual slot, introduced by sead::AudioResourceLoaderCtr
void sead::AudioResourceLoaderCtr::vf_0x18()
{
}

} // namespace sead

// the type information of the classes below AudioResourceLoader
// 0x00829224 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::AudioResourceLoader>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
