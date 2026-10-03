#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_UnitHeapBase.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_LockPolicy_Object.h"

namespace nn {
namespace fnd {
// Instantiations found in the binary:
//   nn::fnd::UnitHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> >  typeinfo 0x008CDEA4  vtable 0x008FC0D8
//
// A UnitHeapBase with a lock (RTTI: the LockObject base is at +0x30). The virtual functions
// forward to UnitHeapBase; FreeV and PrintState lock first. The out-of-line functions are
// instantiated in fnd_UnitHeapTemplate.cpp.
template <typename LockPolicyT>
class UnitHeapTemplate : public UnitHeapBase, private LockPolicyT::LockObject
{
    typedef typename LockPolicyT::LockObject LockObject;
    typedef typename LockObject::ScopedLock ScopedLock;

public:
    // inline (e.g. in __sti___21_fs_UserFileSystem_cpp)
    UnitHeapTemplate(size_t unitSize, uptr address, size_t size, s32 alignment = 4, bit32 option = 0)
    {
        Initialize(unitSize, address, size, alignment, option);
    }
    virtual ~UnitHeapTemplate();

    void Initialize(size_t unitSize, uptr address, size_t size, s32 alignment, bit32 option)
    {
        UnitHeapBase::Initialize(unitSize, address, size, alignment, option);
        LockObject::Initialize();
    }

    void* Allocate();

    // inline (e.g. in nn::fs::CTR::MPCore::detail::FileServerArchive::DeleteObject)
    void Free(void* p)
    {
        ScopedLock lock(*this);
        FreeUnit(p);
    }

    virtual void FreeV(void* p);
    virtual void* GetHeapStart() const;
    virtual size_t GetHeapSize() const;
    virtual void PrintState();
    virtual bool HasAddress(const void* p) const;
};
} // namespace fnd
} // namespace nn
