#include "nn/nex/nex_ConnectionOrientedStream.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_RootTransport.h"

namespace nn {
namespace nex {
namespace {
// the result of the functions the stream does not implement
const int RESULT_NOT_IMPLEMENTED = static_cast<int>(0x80010002);
// a handler is registered already
const int RESULT_ALREADY_REGISTERED = static_cast<int>(0x8001000E);
} // namespace

// 0x003B1D7C | fefates:bytes-fuzzy [tier B]
nn::nex::ConnectionOrientedStream::ConnectionOrientedStream(nn::nex::Stream::Type type, nn::nex::RootTransport* pTransport) : Stream(type)
{
    Network::AcquireInstanceInline();
    m_pEventHandler = nullptr;
    m_pTransport = pTransport;
    m_Port = 0;
}

// 0x003B1E9C slot 0x00 | mk7dlp:callgraph
// 0x003B1E1C (deleting dtor)
nn::nex::ConnectionOrientedStream::~ConnectionOrientedStream()
{
    Network::GetInstance()->ReleaseRef();
}

// 0x003B1C2C slot 0x14 | slot vf_0x14 of nn::nex::ConnectionOrientedStream
bool nn::nex::ConnectionOrientedStream::StartListen(unsigned char port, unsigned char* pPort)
{
    if (!m_pTransport->m_StreamManager.RegisterStream(this, port, &m_Port)) {
        return false;
    }
    if (pPort != nullptr) {
        *pPort = m_Port;
    }
    return true;
}

// 0x003B1BF8 slot 0x18 | slot vf_0x18 of nn::nex::ConnectionOrientedStream
bool nn::nex::ConnectionOrientedStream::StopListen()
{
    if (!m_pTransport->m_StreamManager.UnregisterStream(static_cast<Type>(m_Type), m_Port)) {
        return false;
    }
    m_Port = 0;
    return true;
}

// 0x003B1C70 slot 0x2C (name is ours)
bool nn::nex::ConnectionOrientedStream::OpenEndPoint(nn::nex::EndPoint* pEndPoint)
{
    return false;
}

// 0x003B1CA0 slot 0x30 (name is ours)
nn::nex::EndPoint* nn::nex::ConnectionOrientedStream::FindEndPointByPrincipalID(unsigned principalId)
{
    return nullptr;
}

// 0x003B1CA8 slot 0x34 (name is ours)
u32 nn::nex::ConnectionOrientedStream::GetNumberOfConnectionIDs()
{
    return 0;
}

// 0x003B1C98 slot 0x38 (name is ours)
nn::nex::EndPoint* nn::nex::ConnectionOrientedStream::FindEndPointByConnectionID(unsigned connectionId)
{
    return nullptr;
}

// 0x0072D534 slot 0x3C (name is ours)
nn::nex::qVector<u32> nn::nex::ConnectionOrientedStream::GetConnectionIDs()
{
    return qVector<u32>();
}

// 0x003B1C94 slot 0x44 (name is ours)
void nn::nex::ConnectionOrientedStream::Lock()
{
    // nothing to do
}

// 0x003B1CB0 slot 0x48 (name is ours)
void nn::nex::ConnectionOrientedStream::Unlock()
{
    // nothing to do
}

// 0x003B1BDC slot 0x4C (name is ours)
nn::nex::qResult nn::nex::ConnectionOrientedStream::Initialize()
{
    return qResult(qResult::SUCCESS);
}

// 0x003B1D60 slot 0x50 (name is ours)
nn::nex::qResult nn::nex::ConnectionOrientedStream::Terminate()
{
    return qResult(qResult::SUCCESS);
}

// 0x003B1CD0 slot 0x54 (name is ours)
nn::nex::qResult nn::nex::ConnectionOrientedStream::RegisterEventHandler(nn::nex::TransportEventHandler* pHandler)
{
    if (m_pEventHandler != nullptr) {
        return qResult(RESULT_ALREADY_REGISTERED);
    }
    m_pEventHandler = pHandler;
    return qResult(qResult::SUCCESS);
}

// 0x003B1C78 slot 0x58
nn::nex::qResult nn::nex::ConnectionOrientedStream::SendBroadcast(const nn::nex::StationURL* pURL, nn::nex::Buffer* pPayload)
{
    return qResult(RESULT_NOT_IMPLEMENTED);
}

// 0x003B1D40 slot 0x5C
nn::nex::qResult nn::nex::ConnectionOrientedStream::SendTo(const nn::nex::StationURL* pURL, nn::nex::Buffer* pPayload)
{
    return qResult(RESULT_NOT_IMPLEMENTED);
}

// 0x003B1CB4 slot 0x60
nn::nex::qResult nn::nex::ConnectionOrientedStream::vf_0x60()
{
    return qResult(RESULT_NOT_IMPLEMENTED);
}

// 0x003B1D0C slot 0x64
nn::nex::qResult nn::nex::ConnectionOrientedStream::vf_0x64()
{
    return qResult(RESULT_NOT_IMPLEMENTED);
}

// 0x003B1D5C slot 0x6C
void nn::nex::ConnectionOrientedStream::vf_0x6C()
{
    // nothing to do
}

// 0x003B1D2C slot 0x70 (name is ours)
void nn::nex::ConnectionOrientedStream::RegisterPrincipalID(unsigned principalId, nn::nex::EndPoint* pEndPoint)
{
    // nothing to do
}

// 0x003B1D38 slot 0x74 (name is ours)
bool nn::nex::ConnectionOrientedStream::UnregisterPrincipalID(unsigned principalId, nn::nex::EndPoint* pEndPoint)
{
    return false;
}

// 0x003B1D28 slot 0x78 (name is ours)
void nn::nex::ConnectionOrientedStream::RegisterConnectionID(unsigned connectionId, nn::nex::EndPoint* pEndPoint)
{
    // nothing to do
}

// 0x003B1D30 slot 0x7C (name is ours)
bool nn::nex::ConnectionOrientedStream::UnregisterConnectionID(unsigned connectionId, nn::nex::EndPoint* pEndPoint)
{
    return false;
}

} // namespace nex
} // namespace nn
