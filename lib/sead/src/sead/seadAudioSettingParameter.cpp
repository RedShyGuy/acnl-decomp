#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadTaskParameter.h"
#include "sead/seadAudioSettingParameter.h"

namespace sead {
// 0x0054B178 slot 0x08 | virtual slot, introduced by sead::AudioSettingParameter
void sead::AudioSettingParameter::vf_0x08()
{
}

// 0x0054B174 slot 0x0C | virtual slot, introduced by sead::AudioSettingParameter
void sead::AudioSettingParameter::vf_0x0C()
{
}

// 0x00125C20 | nintendogs:bytes [tier A]
void sead::AudioSettingParameter::appendSubset(sead::AudioSubsetBase*)
{
}

// 0x0012CBD0 | nintendogs:bytes [tier A]
sead::AudioSettingParameter::AudioSettingParameter()
{
}

} // namespace sead

// the type information of the classes below TaskParameter
// 0x00828D9C (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::TaskParameter>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
