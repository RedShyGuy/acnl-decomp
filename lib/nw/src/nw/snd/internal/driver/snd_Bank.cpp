#include "nw/snd/internal/driver/snd_Bank.h"

#include "nw/snd/internal/driver/snd_Channel.h"
#include "nw/snd/internal/driver/snd_NoteOnCallback.h"
#include "nw/snd/internal/snd_BankFileReader.h"
#include "nw/snd/internal/snd_WaveArchiveFileReader.h"
#include "nw/snd/internal/snd_WaveFileReader.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004D3C58 | fefates:bytes [tier B]
f32 nw::snd::internal::driver::Bank::CalcChannelVelocityVolume(unsigned char velocity)
{
    return velocity * (1.0f / 127.0f);
}

// 0x0074168C | nintendogs:callseq [tier A]
nw::snd::internal::driver::Channel* nw::snd::internal::driver::Bank::NoteOn(const BankFileReader& bankReader,
                                                                          const WaveArchiveFileReader& warcReader,
                                                                          const NoteOnInfo& noteOnInfo) const
{
    // the region of the program, key and velocity
    VelocityRegionInfo regionInfo;
    if (!bankReader.ReadVelocityRegionInfo(&regionInfo, noteOnInfo.prgNo, noteOnInfo.key, noteOnInfo.velocity)) {
        return nullptr;
    }
    WaveFileReader waveReader(warcReader.GetWaveFile(regionInfo.waveIndex), 0);
    WaveInfo waveInfo;
    if (!waveReader.ReadWaveInfo(&waveInfo, nullptr)) {
        return nullptr;
    }

    const int channelCount = waveInfo.channelCount > 2 ? 2 : waveInfo.channelCount;
    Channel* pChannel =
        Channel::AllocChannel(channelCount, noteOnInfo.priority, noteOnInfo.channelCallback, noteOnInfo.channelCallbackData);
    if (pChannel == nullptr) {
        return nullptr;
    }
    pChannel->m_Key = noteOnInfo.key;
    pChannel->m_OriginalKey = regionInfo.originalKey;
    pChannel->m_Velocity = CalcChannelVelocityVolume(noteOnInfo.velocity);
    pChannel->m_InstrumentVolume = regionInfo.volume * (1.0f / 127.0f);
    pChannel->m_Tune = regionInfo.pitch;
    pChannel->m_CurveAdshr.SetAttack(regionInfo.adshrCurve.attack);
    pChannel->m_CurveAdshr.SetHold(regionInfo.adshrCurve.hold);
    pChannel->m_CurveAdshr.SetDecay(regionInfo.adshrCurve.decay);
    pChannel->m_CurveAdshr.SetSustain(regionInfo.adshrCurve.sustain);
    pChannel->m_CurveAdshr.SetRelease(regionInfo.adshrCurve.release);
    pChannel->m_InitPan = static_cast<int>(regionInfo.pan + noteOnInfo.initPan - 64) * (1.0f / 63.0f);
    pChannel->m_KeyGroup = regionInfo.keyGroup;
    pChannel->m_IsIgnoreNoteOff = regionInfo.isIgnoreNoteOff;
    pChannel->m_InterpolationType = regionInfo.interpolationType;
    pChannel->Start(waveInfo, noteOnInfo.length, 0);
    return pChannel;
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
