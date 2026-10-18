#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_ConnectionOrientedStream.h"
#include "nn/nex/nex_TransportAdapter.h"

namespace nn {
namespace nex {
namespace {
// a handler is registered already
const int RESULT_ALREADY_REGISTERED = static_cast<int>(0x8001000E);
} // namespace

// 0x003D4DAC | fefates:bytes [tier B]
nn::nex::EndPoint::EndPoint(nn::nex::ConnectionOrientedStream* pStream, const nn::nex::StationURL* pURL)
    : m_URL(*pURL), m_pOwner(nullptr), m_Unknown0x8C(0), m_pEventHandler(nullptr), m_pStream(pStream), m_PrincipalId(0), m_ConnectionId(0),
      m_OpenCount(0), m_Unknown0x84(0)
{
}

// 0x003D4E24 slot 0x00 | mk7dlp:bytes-fuzzy
// 0x003D4DF4 (deleting dtor)
nn::nex::EndPoint::~EndPoint()
{
    if (m_pStream != nullptr && m_ConnectionId != 0) {
        m_pStream->UnregisterConnectionID(m_ConnectionId, this);
    }
    m_ConnectionId = 0;
    if (m_pStream != nullptr && m_PrincipalId != 0) {
        m_pStream->UnregisterPrincipalID(m_PrincipalId, this);
    }
    m_PrincipalId = 0;
    TransportAdapter::s_pInstance->ReleaseAddress(m_URL);
}

// 0x003D4D28 slot 0x38 | slot vf_0x38 of nn::nex::EndPoint
void nn::nex::EndPoint::DisableKeepAlive()
{
    SetKeepAliveTimeout(static_cast<unsigned int>(-1));
}

// 0x003D4D48 slot 0x5C | mk7dlp:callseq-callee
nn::nex::qResult nn::nex::EndPoint::RegisterEventHandler(nn::nex::EndPointEventHandler* pHandler)
{
    if (m_pEventHandler != nullptr) {
        return qResult(RESULT_ALREADY_REGISTERED);
    }
    m_pEventHandler = pHandler;
    return qResult(qResult::SUCCESS);
}

// 0x003D4C68 slot 0x80 (name is ours)
void nn::nex::EndPoint::SetFaultResultHolder(nn::nex::FaultResultHolder* pHolder)
{
    m_pFaultResultHolder = pHolder;
}

// 0x003D4D3C slot 0x84 (name is ours)
void nn::nex::EndPoint::ClearFaultResultHolder()
{
    m_pFaultResultHolder = nullptr;
}

// 0x003D4D38 slot 0x88 | slot vf_0x88 of nn::nex::EndPoint
void nn::nex::EndPoint::SignalFaultEvent(unsigned int error, bool isProcessed)
{
    // nothing to do
}

// 0x003D4C70 | mk7dlp:bytes-fuzzy [tier A]
void nn::nex::EndPoint::SetPrincipalID(unsigned principalId)
{
    if (m_pStream != nullptr) {
        if (m_PrincipalId != 0) {
            m_pStream->UnregisterPrincipalID(m_PrincipalId, this);
        }
        if (principalId != 0) {
            m_pStream->RegisterPrincipalID(principalId, this);
        }
    }
    m_PrincipalId = principalId;
}

// 0x003D4CCC | mk7dlp:bytes-fuzzy [tier A]
void nn::nex::EndPoint::SetConnectionID(unsigned connectionId)
{
    if (m_pStream != nullptr) {
        if (m_ConnectionId != 0) {
            m_pStream->UnregisterConnectionID(m_ConnectionId, this);
        }
        if (connectionId != 0) {
            m_pStream->RegisterConnectionID(connectionId, this);
        }
    }
    m_ConnectionId = connectionId;
}

// 0x003D4D84 | mk7dlp:bytes [tier A]
void nn::nex::EndPoint::Open()
{
    m_pStream->OpenEndPoint(this);
}

// 0x003D4D98 | fefates:callgraph [tier C]
void nn::nex::EndPoint::Close()
{
    m_pStream->CloseEndPoint(this);
}

} // namespace nex
} // namespace nn
