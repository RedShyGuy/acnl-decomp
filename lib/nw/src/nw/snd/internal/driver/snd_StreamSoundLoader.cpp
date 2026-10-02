#include "nw/snd/internal/driver/snd_StreamSoundLoader.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004C6858 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::RequestLoadHeader()
{
}

// 0x004CEC48 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::Initialize()
{
}

// 0x004CEC90 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::LoadHeader1(nw::snd::internal::DriverCommandStreamSoundLoadHeader*)
{
}

// 0x004CEFB0 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::LoadHeader2(nw::snd::internal::DriverCommandStreamSoundLoadHeader*)
{
}

// 0x004CF194 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::WaitFinalize()
{
}

// 0x004CF358 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::ApplyStartOffset(unsigned int, unsigned int*)
{
}

// 0x004CF490 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::ChangeRegionInfo(unsigned int)
{
}

// 0x004CF728 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::ReadTrackInfoFromStreamSoundFile(nw::snd::internal::StreamSoundFileReader&)
{
}

// 0x004CF88C | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::Update()
{
}

// 0x004CF910 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::LoadData1(nw::snd::internal::DriverCommandStreamSoundLoadData*, void**, unsigned int, unsigned int, unsigned int)
{
}

// 0x004D00EC | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::LoadData2(nw::snd::internal::DriverCommandStreamSoundLoadData*, void**, unsigned int, unsigned int, unsigned int)
{
}

// 0x004D058C | fefates:bytes [tier B]
nw::snd::internal::driver::StreamSoundLoader::~StreamSoundLoader()
{
}

// 0x00741534 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundLoader::IsBusy() const
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
