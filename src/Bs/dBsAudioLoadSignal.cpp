#include "Bs/dBsAudioLoadSignal.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Audio/dAudioObjFurnitureBase.h"
#include "Sound/dSoundMgr.h"
#include "netgame/netgame_Api.h"

// 0x002C55D4
oml::framework::Result BsAudioLoadSignal::Initialize()
{
    if (!m_IsLoading) {
        SoundMgr::SLoadCurrentStageGroups();
        m_IsLoading = 1;
    }
    if (!SoundMgr::SIsStageGroupLoaded()) {
        return oml::framework::RESULT_RETRY;
    }
    AudioObjFurnitureBase::s_IsLoadSignalOn = true;
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        m_HasMix29 = 1;
        audio::bgm::GetTrackMixer()->AddMix(29);
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x002C5640
oml::framework::Result BsAudioLoadSignal::Finalize()
{
    if (!SoundMgr::SIsStageGroupLoaded()) {
        return oml::framework::RESULT_RETRY;
    }
    SoundMgr::SUnloadCurrentStageGroups();
    AudioObjFurnitureBase::s_IsLoadSignalOn = false;
    if (m_HasMix29) {
        audio::bgm::GetTrackMixer()->RemoveMix(29);
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x002C568C
BsAudioLoadSignal::BsAudioLoadSignal() : m_IsLoading(0), m_HasMix29(0) {}

// 0x002C56C0
// 0x002C56B0 (deleting dtor)
BsAudioLoadSignal::~BsAudioLoadSignal()
{
    // (only the base is destroyed)
}
