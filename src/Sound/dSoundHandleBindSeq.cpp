#include "Sound/dSoundHandleBindSeq.h"
#include "Sound/dSoundMgr.h"
#include "nw/snd/internal/snd_SequenceSound.h"

// 0x002E837C
SoundHandleBindSeq::SoundHandleBindSeq()
{
    // (only the members are constructed)
}

// 0x002E83C0
// 0x002E8394 (deleting dtor)
SoundHandleBindSeq::~SoundHandleBindSeq()
{
    // (only the members are destroyed)
}

// 0x002E82EC
void SoundHandleBindSeq::BindSequence()
{
    if (m_pSound == NULL) {
        return;
    }
    u32 id = m_pSound->m_Id;
    if (id == 0xFFFFFFFF) {
        return;
    }
    if (SoundMgr::s_pInstance->m_pArchive->GetSoundType(id) != nw::snd::SoundArchive::SOUND_TYPE_SEQ) {
        return;
    }
    m_SequenceHandle.detail_AttachSoundAsTempHandle(nw::snd::internal::DynamicCast<nw::snd::internal::SequenceSound>(m_pSound));
}
