#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class EndPoint;

// An observer of the calls the client broker dispatches (the class and all names are ours). Nothing in
// the binary sets the instance (0x009712E0 is only read), so the functions do nothing.
class ProtocolCallObserver
{
public:
    virtual ~ProtocolCallObserver() {}  // slots 0x00/0x04 (assumed)
    virtual void vf_0x08() = 0;
    virtual void OnCallBegins() = 0;                        // slot 0x0C
    virtual void OnCallEnds(nn::nex::EndPoint* pEndPoint) = 0; // slot 0x10
    virtual bool IsEnabled() = 0;                           // slot 0x14

    // around the dispatch of a request (under the lock of the scheduler)
    static void NotifyCallBegins(); // 0x0039A4FC (name is ours)
    static void NotifyCallEnds(nn::nex::EndPoint* pEndPoint); // 0x0039A554 (name is ours)

    static ProtocolCallObserver* s_pInstance;
};
} // namespace nex
} // namespace nn
