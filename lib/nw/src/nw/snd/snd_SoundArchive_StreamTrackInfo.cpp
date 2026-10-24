#include "nw/snd/snd_SoundArchive_StreamTrackInfo.h"

#include <cstring>

// 0x004C01D8 | fefates:bytes [tier B]
nw::snd::SoundArchive::StreamTrackInfo::StreamTrackInfo()
    : volume(0), pan(0), span(0), flags(0), mainSend(127), lpfFreq(64), biquadType(0), biquadValue(0), channelCount(0)
{
    std::memset(globalChannelIndex, 0xFF, sizeof(globalChannelIndex));
    fxSend[0] = 0;
    fxSend[1] = 0;
}
