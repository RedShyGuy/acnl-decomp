#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAudioFx.h"
#include "sead/seadAudioFxCtr.h"
#include "sead/seadAudioFxHolder.h"

namespace sead {
// 0x00132CEC | nintendogs:bytes [tier B]
sead::AudioFxCtr::AudioFxCtr() : AudioFx(), mDspParam(nullptr), mType(TYPE_NW)
{
}

// 0x00141B98 slot 0x08
// 0x0053E214 | nintendogs:bytes (deleting dtor)
sead::AudioFxCtr::~AudioFxCtr()
{
    if (mHolder != nullptr) {
        delete mHolder;
        mHolder = nullptr;
    }
}
} // namespace sead

// the type information of the classes below AudioFx
// 0x0082971C (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::AudioFx>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
