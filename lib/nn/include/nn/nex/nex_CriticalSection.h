#pragma once

#include "decomp.h"
#include "nn/nex/nex_LockChecker.h"
#include "nn/nex/nex_MutexPrimitive.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// A lock of nex. Enter and Leave do nothing while s_IsLockDisabled is set (single threaded
// operation), a ForcedCriticalSection always locks. The names of the functions other than the
// constructor and of the members are ours.
// RTTI N2nn3nex15CriticalSectionE @ 0x008CE478
// vtable 0x008FCE60 (vptr 0x008FCE68), offset_to_top 0, 5 entries
class CriticalSection : public ::nn::nex::RootObject
{
public:
    explicit CriticalSection(unsigned int level); // 0x00377580 | fefates:callgraph [tier C]
    virtual ~CriticalSection(); // 0x00377608 slot 0x00
    // 0x003775DC slot 0x04 (deleting dtor)
    virtual void Enter(); // 0x00377540 slot 0x08
    virtual void Leave(); // 0x00377560 slot 0x0C
    // (inline in the original, with a copy out of line)
    virtual bool IsForced(); // 0x0072B6C8 slot 0x10

    MutexPrimitive m_Mutex;      // 0x04
    LockChecker m_LockChecker;   // 0x14

    static bool s_IsLockDisabled;
};
ASSERT_SIZE(CriticalSection, 0x28);

// Enters a CriticalSection for its scope (the class name is from mk7dlp, its functions are inline).
class ScopedCS
{
public:
    explicit ScopedCS(CriticalSection& criticalSection) : m_pCriticalSection(&criticalSection) { m_pCriticalSection->Enter(); }
    ~ScopedCS() { m_pCriticalSection->Leave(); }

    CriticalSection* m_pCriticalSection; // 0x00
};
} // namespace nex
} // namespace nn
