#include "Sound/dSoundObjFurnitureInsect.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"

namespace {
const s32 VARIABLE_SINGING = 13;
} // namespace

// 0x00335B68
SoundObjFurnitureInsect::SoundObjFurnitureInsect() : m_IsSinging(false)
{
}

// 0x002D3158
// 0x00335B88 (deleting dtor)
SoundObjFurnitureInsect::~SoundObjFurnitureInsect()
{
    // (only the base is destroyed)
}

// 0x00335AB4
void SoundObjFurnitureInsect::Update(const nn::math::VEC3* world, const nn::math::VEC3* relative, f32 frame)
{
    SoundObjFurniture::Update(world, relative, frame);
    u32 soundId = m_pInfo->soundLoop;
    for (s32 i = 0; i < 2; i++) {
        if (m_HandleSet.m_Handles[i].GetId() == soundId) {
            sead::SoundHandle* handle = &m_HandleSet.m_Handles[i];
            if (handle == NULL) {
                break;
            }
            nw::snd::SequenceSoundHandle sequence(handle);
            s16 value = 0;
            if (sequence.m_pSound != NULL) {
                sequence.m_pSound->ReadVariable(VARIABLE_SINGING, &value);
            }
            m_IsSinging = value > 0;
            return;
        }
    }
    m_IsSinging = false;
}
