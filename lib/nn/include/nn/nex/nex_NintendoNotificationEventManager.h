#pragma once

#include "decomp.h"
#include "nn/nex/nex__Proto_NintendoNotificationEventProtocolServer.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
// A handler of the Nintendo notification events (no class of the binary is one; the name is ours).
class NintendoNotificationEventHandler
{
public:
    virtual void vf_0x00();
    virtual void vf_0x04();
    virtual void ProcessNintendoNotificationEvent(const nn::nex::NintendoNotificationEvent& event); // slot 0x08
};
} // namespace nex
} // namespace nn

namespace __rw {
// (the original calls erase(first, last) of the handlers out of line; defined in
// nex_NintendoNotificationEventManager.cpp)
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*,
                          __ident<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*>, std::less<nn::nex::NintendoNotificationEventHandler*>,
                          nn::nex::MemAllocator<nn::nex::NintendoNotificationEventHandler*> >::iterator
__rb_tree<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*,
          __ident<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*>, std::less<nn::nex::NintendoNotificationEventHandler*>,
          nn::nex::MemAllocator<nn::nex::NintendoNotificationEventHandler*> >::erase(iterator first, iterator last);
} // namespace __rw

namespace nn {
namespace nex {
// The server of the Nintendo notification events: the events go to the parent manager (the clone
// of a manager has it) or to the handlers. The names of the members are ours.
// RTTI N2nn3nex32NintendoNotificationEventManagerE @ 0x008CF29C
// vtable 0x008FF5E0 (vptr 0x008FF5E8), offset_to_top 0, 24 entries
class NintendoNotificationEventManager : public ::nn::nex::_Proto_NintendoNotificationEventProtocolServer
{
public:
    explicit NintendoNotificationEventManager(nn::nex::NintendoNotificationEventManager* pParent); // 0x003CA6A0 | fefates:bytes [tier B]
    virtual ~NintendoNotificationEventManager(); // 0x003CA7E0 slot 0x00 | fefates:bytes
    // 0x003CA7D0 slot 0x04 | fefates:callseq (deleting dtor)
    // (inline in the original, with a copy out of line)
    virtual nn::nex::Protocol* Clone(); // 0x0072DF00 slot 0x4C
    virtual void ProcessNintendoNotificationEvent(const nn::nex::NintendoNotificationEvent& event); // 0x003CA5F4 slot 0x5C | fefates:bytes

    std::set<NintendoNotificationEventHandler*> m_Handlers; // 0x40
    NintendoNotificationEventManager* m_pParent;            // 0x5C
};
ASSERT_OFFSET(NintendoNotificationEventManager, m_pParent, 0x5C);
ASSERT_SIZE(NintendoNotificationEventManager, 0x60);
} // namespace nex
} // namespace nn
