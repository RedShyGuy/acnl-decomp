#pragma once

#include "decomp.h"
#include "nn/nex/nex_ProtocolRequestBrokerInterface.h"
#include "nn/nex/nex_ServerProtocol.h"
#include "nn/nex/nex_Operation.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex27CallProtocolMethodOperationE @ 0x008CEFE8
// vtable 0x008FEEB0 (vptr 0x008FEEB8), offset_to_top 0, 6 entries
class CallProtocolMethodOperation : public ::nn::nex::Operation
{
public:
    // the type number of the operation
    static const u32 TYPE = 19;

    // (inline in JobProcessProtocolMessage)
    CallProtocolMethodOperation() : Operation(0), m_AttemptCount(0), m_Unknown0x14(false), m_Unknown0x18(0), m_Unknown0x1C(0), m_pMessage(nullptr) {}
    virtual ~CallProtocolMethodOperation(); // 0x003BC2F8 slot 0x00
    // 0x003BC2E8 (deleting dtor)
    virtual u32 GetTypeImpl() const; // 0x0072DE18 slot 0x0C
    virtual const wchar_t* GetName() const; // 0x0072DDE8 slot 0x10
    virtual void vf_0x14(); // 0x003BC2E4 slot 0x14
    // the next attempt of the call: its result is reset to success
    void InitializeForNewCallAttempt(); // 0x003BC298 | mk7dlp:bytes [tier A]

    // (only the members other code uses are known; the names are ours)
    u32 m_AttemptCount;           // 0x10
    bool m_Unknown0x14;           // 0x14
    u32 m_Unknown0x18;            // 0x18
    u32 m_Unknown0x1C;            // 0x1C
    qResult m_Result;             // 0x20 (of the call)
    void* m_pMessage;             // 0x2C
};
ASSERT_SIZE(CallProtocolMethodOperation, 0x30);
// (inline; ServerProtocol.cpp) the result of the call being dispatched
inline void ServerProtocol::SetCallErrorInline(const nn::nex::qResult& result)
{
    static_cast<CallProtocolMethodOperation*>(m_pBroker->m_OperationManager.GetCurrentOperation())->m_Result = result;
}
} // namespace nex
} // namespace nn
