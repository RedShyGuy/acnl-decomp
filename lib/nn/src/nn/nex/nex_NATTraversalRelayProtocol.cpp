#include "nn/nex/nex_NATTraversalRelayProtocol.h"

namespace nn {
namespace nex {
// 0x003B83E8 slot 0x60
void nn::nex::NATTraversalRelayProtocol::InitiateProbe(const nn::nex::StationURL& url)
{
    // nothing to do
}

// 0x003B83EC slot 0x6C (name is ours)
void nn::nex::NATTraversalRelayProtocol::ReportNATProperties(const unsigned int& mapping, const unsigned int& filtering, const unsigned int& rtt)
{
    // nothing to do
}

// 0x003B83F0 slot 0x70 (name is ours)
void nn::nex::NATTraversalRelayProtocol::GetRelayParameters(unsigned int* pMode, nn::nex::DateTime* pTime, nn::nex::String* pAddress, unsigned short* pPort, unsigned int* pAddressType,
                                                            unsigned int* pServerTime)
{
    // nothing to do
}

// 0x003B83F4 slot 0x78 (name is ours)
void nn::nex::NATTraversalRelayProtocol::SetDefaultCredentials(nn::nex::Credentials* pCredentials)
{
    m_pClient->SetDefaultCredentials(pCredentials);
}

// 0x003B8404 slot 0x5C
void nn::nex::NATTraversalRelayProtocol::RequestProbeInitiation(const nn::nex::qList<nn::nex::StationURL>& targets)
{
    // nothing to do
}

// 0x003B8408 slot 0x68
void nn::nex::NATTraversalRelayProtocol::ReportNATTraversalResult(const unsigned int& cid, const bool& isSucceeded, const unsigned int& rtt)
{
    // nothing to do
}

// 0x003B840C slot 0x64
void nn::nex::NATTraversalRelayProtocol::RequestProbeInitiationExt(const nn::nex::qList<nn::nex::StationURL>& targets, const nn::nex::StationURL& source)
{
    // nothing to do
}

// 0x003B8410 slot 0x74 (name is ours)
void nn::nex::NATTraversalRelayProtocol::ReportNATTraversalResultDetail(const unsigned int& cid, const bool& isSucceeded, const nn::nex::NATTraversalResult& detail, const unsigned int& rtt)
{
    // nothing to do
}

// 0x003B846C slot 0x00
// 0x003B8414 (deleting dtor)
nn::nex::NATTraversalRelayProtocol::~NATTraversalRelayProtocol()
{
    if (m_pClient != nullptr) {
        delete m_pClient;
        m_pClient = nullptr;
    }
}

} // namespace nex
} // namespace nn
