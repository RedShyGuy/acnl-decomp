#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class StreamSoundLoader
{
public:
    class StreamDataLoadTask;
    class StreamHeaderLoadTask;
    void RequestLoadHeader(); // 0x004C6858 | fefates:bytes [tier B]
    void Initialize(); // 0x004CEC48 | fefates:bytes [tier B]
    void LoadHeader1(nw::snd::internal::DriverCommandStreamSoundLoadHeader*); // 0x004CEC90 | fefates:bytes [tier B]
    void LoadHeader2(nw::snd::internal::DriverCommandStreamSoundLoadHeader*); // 0x004CEFB0 | fefates:bytes [tier B]
    void WaitFinalize(); // 0x004CF194 | fefates:bytes [tier B]
    void ApplyStartOffset(unsigned int, unsigned int*); // 0x004CF358 | fefates:bytes [tier B]
    void ChangeRegionInfo(unsigned int); // 0x004CF490 | fefates:bytes [tier B]
    void ReadTrackInfoFromStreamSoundFile(nw::snd::internal::StreamSoundFileReader&); // 0x004CF728 | fefates:bytes [tier B]
    void Update(); // 0x004CF88C | fefates:bytes [tier B]
    void LoadData1(nw::snd::internal::DriverCommandStreamSoundLoadData*, void**, unsigned int, unsigned int, unsigned int); // 0x004CF910 | fefates:bytes [tier B]
    void LoadData2(nw::snd::internal::DriverCommandStreamSoundLoadData*, void**, unsigned int, unsigned int, unsigned int); // 0x004D00EC | fefates:bytes [tier B]
    ~StreamSoundLoader(); // 0x004D058C | fefates:bytes [tier B]
    void IsBusy() const; // 0x00741534 | fefates:bytes [tier B]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
