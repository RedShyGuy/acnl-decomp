#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
class AuthenticationClient;
class CallContext;
class Data;
class RVConnectionData;
class String;
class Ticket;
class qResult;
template <typename T0, typename T1>
class AnyObjectHolder;

// The tickets of the stations (by their principal id) of an AuthenticationClient; it gets new ones
// from the ticket granting server. The names of the members are ours.
class TicketManager : public RootObject
{
public:
    explicit TicketManager(nn::nex::AuthenticationClient* pClient); // 0x0036F244 | mk7dlp:bytes [tier A]
    void InsertTicket(unsigned principalId, nn::nex::Ticket* pTicket); // 0x0036EC1C | mk7dlp:bytes [tier A]
    // the ticket for the target (from the server if there is none yet; the job holds a reference)
    bool AcquireTicket(nn::nex::CallContext* pContext, unsigned int sourceId, unsigned int targetId, nn::nex::Ticket** ppTicket); // 0x0036ED64 | fefates:callgraph [tier C]
    bool LoginWithData(nn::nex::CallContext* pContext, nn::nex::qResult* pResult, const char* pPassword, unsigned int* pPrincipalId, nn::nex::RVConnectionData* pConnectionData,
                       const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>* pData); // 0x0036EEE4
    // the reference is released (the ticket is forgotten when only the manager holds it)
    void ReleaseTicket(nn::nex::Ticket* pTicket); // 0x0036F098 | fefates:bytes [tier B]

    u32 m_Unknown0x00;                   // 0x00 (not set)
    qMap<u32, Ticket*> m_Tickets;        // 0x04
    AuthenticationClient* m_pClient;     // 0x20
};
ASSERT_SIZE(TicketManager, 0x24);
} // namespace nex
} // namespace nn

namespace __rw {
// (the range erase of the tickets is out of line; nex_TicketManager.cpp)
template <>
DECOMP_NOINLINE __rb_tree<u32, std::pair<const u32, nn::nex::Ticket*>, __select1st<std::pair<const u32, nn::nex::Ticket*>, u32>, std::less<u32>,
                          nn::nex::MemAllocator<std::pair<const u32, nn::nex::Ticket*> > >::iterator
__rb_tree<u32, std::pair<const u32, nn::nex::Ticket*>, __select1st<std::pair<const u32, nn::nex::Ticket*>, u32>, std::less<u32>,
          nn::nex::MemAllocator<std::pair<const u32, nn::nex::Ticket*> > >::erase(iterator first, iterator last);
} // namespace __rw
