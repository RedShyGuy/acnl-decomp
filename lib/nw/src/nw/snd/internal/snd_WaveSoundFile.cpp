#include "nw/snd/internal/snd_WaveSoundFile.h"

namespace nw {
namespace snd {
namespace internal {
// 0x009828A0
const AdshrCurve WaveSoundFile::DEFAULT_ADSHR_CURVE(127, 127, 127, 127, 127);

// sets WaveSoundFile::DEFAULT_ADSHR_CURVE
// 0x0079C380 (static initializer)

} // namespace internal
} // namespace snd
} // namespace nw
