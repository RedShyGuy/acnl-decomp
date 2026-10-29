#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAudioSettingParameter.h"
#include "sead/seadTaskParameter.h"

namespace sead {
// 0x00125C20 | nintendogs:bytes [tier A]
void sead::AudioSettingParameter::appendSubset(AudioSubsetBase* subset)
{
    mSubsets.pushBack(subset);
}

// 0x0012CBD0
sead::AudioSettingParameter::AudioSettingParameter()
    : TaskParameter(), mSystem(nullptr), mResetter(nullptr), mPlayer(nullptr), mResourceLoader(nullptr), mSubsets()
{
    mSubsets.initOffset(offsetof(AudioSubsetBase, mNode));
}

// 0x0054B178 slot 0x08
// 0x0054B174 (deleting dtor)
sead::AudioSettingParameter::~AudioSettingParameter()
{
    // empty (in the original too)
}
} // namespace sead

// the type information of the classes below TaskParameter
// 0x00828D9C (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::TaskParameter>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
