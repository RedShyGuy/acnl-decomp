#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
class EndPoint;
class StreamManager;

// RTTI N2nn3nex11CredentialsE @ 0x008CE000
// vtable 0x008FC270 (vptr 0x008FC278), offset_to_top 0, 2 entries
class Credentials : public ::nn::nex::RefCountedObject
{
public:
    // (inline)
    Credentials() : m_PrincipalId(0), m_ConnectionId(0), m_Password(""), m_pStreamManager(nullptr), m_pSecureConnection(nullptr), m_pConnection(nullptr) {}
    virtual ~Credentials(); // 0x00357650 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003575E0 slot 0x04 | fefates:bytes (deleting dtor)
    void MarkInvalid(); // 0x00137B04 | mk7dlp:bytes [tier B]
    // the connection of the stream type (none for 0)
    nn::nex::EndPoint* GetConnection(unsigned short streamType) const; // 0x0072A100 | fefates:bytes [tier B]

    // (inline; the name is ours) forgets everything
    void Clear()
    {
        if (m_pStreamManager != nullptr) {
            m_pStreamManager = nullptr;
        }
        m_PrincipalId = 0;
        m_ConnectionId = 0;
        m_pSecureConnection = nullptr;
        m_pConnection = nullptr;
        m_Name = "";
        m_Password = "";
    }

    // the stream type of the secure connection
    static const u16 STREAM_TYPE_SECURE = 10;

    // (the names are ours)
    u32 m_PrincipalId;              // 0x0C
    u32 m_ConnectionId;             // 0x10
    String m_Name;                  // 0x14
    String m_Password;              // 0x1C
    StreamManager* m_pStreamManager; // 0x24 (of the back end)
    EndPoint* m_pSecureConnection;  // 0x28 (of stream type 10)
    EndPoint* m_pConnection;        // 0x2C
};
ASSERT_SIZE(Credentials, 0x30);
} // namespace nex
} // namespace nn
