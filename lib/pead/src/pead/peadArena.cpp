#include "pead/peadArena.h"

namespace pead {
// 0x0097FA1C
bool Arena::s_IsDeviceMemoryUsed = false;

// 0x0053B87C (name is ours)
void pead::Arena::destroy()
{
    mSize = 0;
    mStart = nullptr;
    s_IsDeviceMemoryUsed = false;
    mInitialized = false;
}

// 0x0053B89C (name after sead)
pead::Arena::Arena() : mStart(nullptr), mSize(0), mInitialized(false)
{
}

// 0x0053B8B0 (name after sead)
pead::Arena::~Arena()
{
    // empty (in the original too)
}
} // namespace pead
