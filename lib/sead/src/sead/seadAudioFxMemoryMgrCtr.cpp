#include "sead/seadAudioFxMemoryMgr.h"
#include "sead/seadAudioFxMemoryMgrCtr.h"

namespace sead {
// (the constructor of AudioFxMemoryMgr is out of line in the original: 0x0013C69C)
// 0x00138D5C (name is ours)
sead::AudioFxMemoryMgrCtr::AudioFxMemoryMgrCtr() : AudioFxMemoryMgr()
{
}

// 0x00546624 slot 0x00
// 0x0054A140 (deleting dtor)
sead::AudioFxMemoryMgrCtr::~AudioFxMemoryMgrCtr()
{
    // empty (in the original too)
}
} // namespace sead
