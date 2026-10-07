#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalMessage.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local25LocalUpdateSessionMessageE @ 0x008CFCC4
// vtable 0x00901170 (vptr 0x00901178), offset_to_top 0, 4 entries
//
// The node table of the host (LocalNetworkManager::SendUpdateSessionMessage): the header has the
// version of the table, a value of the session and the participation state; the data are the
// node ids (and with the host migration those of LocalMigrationManager). The member names are
// ours.
class LocalUpdateSessionMessage : public ::nn::pia::local::LocalMessage
{
public:
    // the header in the buffer
    struct UpdateSessionHeader
    {
        LocalMessage::Header m_Header; // 0x00
        u32 m_Version;                 // 0x0C
        u32 m_Unknown0xC;              // 0x10, LocalNetworkManager 0x123C
        u8 m_ParticipationState;       // 0x14
        u8 m_Reserved[7];              // 0x15
    };

    static const u16 UPDATE_SESSION_HEADER_SIZE = 28;

    LocalUpdateSessionMessage(u8* pBuffer, u32 bufferSize, u32 version); // 0x00420B28
    virtual ~LocalUpdateSessionMessage(); // 0x00420B5C slot 0x00
    // 0x00420B58 slot 0x04 (deleting dtor)
    virtual void UpdateMessageHeader(); // 0x00420AAC slot 0x08 | fefates:callseq
    virtual bool ParseMessageHeader(); // 0x00420A50 slot 0x0C | fefates:bytes

    u32 m_Version;            // 0x14
    u32 m_Unknown0x18;        // 0x18
    u8 m_ParticipationState;  // 0x1C
};
ASSERT_SIZE(LocalUpdateSessionMessage::UpdateSessionHeader, 28);
ASSERT_SIZE(LocalUpdateSessionMessage, 0x20);
} // namespace local
} // namespace pia
} // namespace nn
