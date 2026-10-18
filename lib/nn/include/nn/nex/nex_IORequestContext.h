#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_UserContext.h"

namespace nn {
namespace nex {
// An I/O request of an end point (the connection or the disconnection): its completion function and
// the buffer of the reply. The names of the members are ours.
// RTTI N2nn3nex16IORequestContextE @ 0x008CE584
// vtable 0x008FD0E0 (vptr 0x008FD0E8), offset_to_top 0, 2 entries
class IORequestContext : public ::nn::nex::RootObject
{
public:
    // (inline)
    IORequestContext() : m_pReplyData(nullptr), m_pConnectData(nullptr), m_pCallback(nullptr) { m_Context.m_Value = 0; }
    virtual ~IORequestContext(); // 0x0037F25C slot 0x00
    // 0x0037F1CC slot 0x04 (deleting dtor)

    // (inline) forgets the request
    void Clear()
    {
        if (m_pConnectData != nullptr) {
            m_pConnectData->ReleaseRef();
        }
        m_pConnectData = nullptr;
        m_pReplyData = nullptr;
        m_pCallback = nullptr;
    }

    Buffer* m_pReplyData;                    // 0x04
    Buffer* m_pConnectData;                  // 0x08 (a reference; the data of the connection request)
    EndPoint::CompletionCallback m_pCallback; // 0x0C
    UserContext m_Context;                   // 0x10
};
ASSERT_SIZE(IORequestContext, 0x14);
} // namespace nex
} // namespace nn
