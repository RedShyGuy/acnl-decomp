#include "sead/seadArena.h"
#include "nn/os/os_Api.h"

namespace sead {
// 0x0097F874
bool sead::Arena::s_IsDeviceMemoryUsed = false;

// 0x0055D0F8 (name is ours)
sead::Arena::Arena() : mStart(nullptr), mSize(0), mIsInitialized(false)
{
}

// 0x0055D10C (name is ours)
sead::Arena::~Arena()
{
    // empty (in the original too)
}

// 0x00139110 | nintendogs:bytes [tier A]
void sead::Arena::initialize(u32 size)
{
    s_IsDeviceMemoryUsed = true;
    mSize = size;
    mStart = reinterpret_cast<u8*>(nn::os::GetDeviceMemoryAddress());
}

} // namespace sead
