#include "Audio/dAudioObjFgBase.h"
#include "Audio/dAudioObjRiver.h"
#include "Audio/dAudioObjFall.h"
#include "Audio/dAudioObjPond.h"

namespace {
// 0x008462F8
AudioObjFgBase* (* const s_FgCreateFuncs[AudioObjFgBase::KIND_NUM])(const SoundCreateArg*) = {
    &AudioObjRiver::Create,
    &AudioObjFall::Create,
    &AudioObjPond::Create,
};
} // namespace

// 0x0024FCC8
AudioObjFgBase* AudioObjFgBase::Create(const SoundCreateArg* arg, u32 kind)
{
    return s_FgCreateFuncs[kind](arg);
}

// 0x0024FCD8
void AudioObjFgBase::Destroy(AudioObjFgBase* obj)
{
    if (obj != NULL) {
        delete obj;
    }
}
