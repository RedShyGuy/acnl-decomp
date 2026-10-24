#include "nw/snd/internal/snd_StreamDataInfoDetail.h"

#include "nw/snd/internal/snd_WaveFileReader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C9304 | fefates:bytes [tier B]
void nw::snd::internal::StreamDataInfoDetail::SetStreamSoundInfo(const StreamSoundFile::StreamSoundInfo& info)
{
    sampleFormat = WaveFileReader::GetSampleFormat(info.encoding);
    sampleRate = info.sampleRate;
    isLoop = info.isLoop != 0;
    loopStart = info.loopStart;
    frameCount = info.frameCount;
    originalLoopStart = info.originalLoopStart;
    originalLoopEnd = info.originalLoopEnd;
    blockSampleCount = info.blockSampleCount;
    blockSize = info.blockSize;
    lastBlockSampleCount = info.lastBlockSampleCount;
    lastBlockSize = info.lastBlockPaddedSize;
}

} // namespace internal
} // namespace snd
} // namespace nw
