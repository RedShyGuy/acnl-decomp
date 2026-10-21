#pragma once

#include "decomp.h"
#include "nn/nex/nex_SystemComponent.h"
#include "nn/nex/nex_qMap.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class CallContext;
} // namespace nex
} // namespace nn

namespace std {
// (Register has the insert of this instance inline)
template <>
inline nn::nex::CallContext*& map<u32, nn::nex::CallContext*, less<u32>, nn::nex::MemAllocator<pair<const u32, nn::nex::CallContext*> > >::operator[](const u32& key)
{
    return IndexInline(key);
}
} // namespace std

namespace nn {
namespace nex {
class PeriodicJob;

// The running CallContexts by their ids, and the ids associated with them (cancelled with them).
// A periodic job fails the calls whose deadline passed. The names of the members and of the
// functions without a reference name are ours.
// RTTI N2nn3nex19CallContextRegisterE @ 0x008CE790
// vtable 0x008FD5F8 (vptr 0x008FD600), offset_to_top 0, 16 entries
class CallContextRegister : public ::nn::nex::SystemComponent
{
public:
    CallContextRegister(); // 0x003924AC | fefates:bytes [tier B]
    virtual ~CallContextRegister(); // 0x003926A0 slot 0x00 | fefates:callseq
    // 0x00392670 slot 0x04 (deleting dtor)
    // (inline in the original, with copies out of line)
    virtual const wchar_t* GetType() const; // 0x0072CBA8 slot 0x08
    virtual bool IsAKindOf(const wchar_t* pType) const; // 0x0072CBD8 slot 0x0C
    virtual void vf_0x10(); // 0x00392234 slot 0x10
    virtual bool BeginInitialization(); // 0x003921F8 slot 0x20 (slot name from the reference symbols)
    virtual bool BeginTermination(); // 0x00391D20 slot 0x28 | fefates:bytes

    void Unregister(unsigned int id); // 0x00391A54 | fefates:bytes [tier B]
    nn::nex::CallContext* GetCallContext(unsigned int id); // 0x00391B78 | fefates:bytes [tier B]
    void GetAssociatedId(unsigned int id, nn::nex::qVector<unsigned int>* pIds); // 0x00391BD8 | fefates:bytes [tier B]
    // cancels the running calls of the owner (or the owner id) and the calls associated with them
    void CancelCallContexts(void* pOwner, unsigned int ownerId); // 0x00391DA8 | fefates:bytes-fuzzy [tier B]
    // fails the running calls whose deadline passed (and cancels the associated ones; the job)
    void CheckExpiredCalls(int unused); // 0x00391FD4 (name from the debug string of its job)
    void AssociateCallContext(unsigned int id, unsigned int associatedId); // 0x00392208 | fefates:bytes [tier B]
    // starts the periodic job of CheckExpiredCalls
    void Start(); // 0x00392238 | fefates:bytes [tier B]
    // the id of the call context
    unsigned int Register(nn::nex::CallContext* pCallContext); // 0x00392350 | fefates:bytes [tier B]

    qMap<u32, CallContext*> m_CallContexts;     // 0x20
    std::multimap<u32, u32> m_Associations;     // 0x3C
    u32 m_NextId;                               // 0x58
    PeriodicJob* m_pTimeoutJob;                 // 0x5C

    // (set by the Core; name is ours)
    static bool s_IsCancelDisabled;
};
ASSERT_SIZE(CallContextRegister, 0x60);
} // namespace nex
} // namespace nn
