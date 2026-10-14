#include "nn/nex/nex_Packet.h"

namespace nn {
namespace nex {
// 0x003D03CC | fefates:callgraph [tier C]
nn::nex::Packet::Packet()
    : m_pChainNext(nullptr), m_pChainPrev(nullptr), m_Unknown0x14(0), m_Unknown0x18(0), m_Unknown0x1C(0), m_Unknown0x20(0),
      m_MessageVersion(0), m_SourcePort(0), m_DestinationPort(0), m_TypeFlags(0), m_Unknown0x64(0),
      m_SessionId(0), m_Unknown0x66(0), m_Unknown0x67(0), m_SubstreamId(0), m_Unknown0x86(0), m_Unknown0x88(0),
      m_IsHeaderV1Valid(false), m_Unknown0x8C(0), m_pPayload(nullptr)
{
    m_PacketSignature.m_Size = 0;
    m_Signature.m_Size = 0;
}

// 0x003D0560 slot 0x00
// 0x003D04C0 (deleting dtor)
nn::nex::Packet::~Packet()
{
    if (m_pPayload != nullptr) {
        m_pPayload->ReleaseRef();
    }
}

} // namespace nex
} // namespace nn
