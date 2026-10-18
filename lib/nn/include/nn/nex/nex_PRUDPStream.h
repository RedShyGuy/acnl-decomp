#pragma once

#include "decomp.h"
#include "nn/nex/nex_ConnectionOrientedStream.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_LogicalClockTmpl.h"
#include "nn/nex/nex_PRUDPEndPointMap.h"
#include "nn/nex/nex_TimeoutManager.h"
#include "nn/nex/nex_TransportSignatureGenerator.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qMap.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class Key;
class Packet;
class PacketOut;
class PRUDPEndPoint;
class PRUDPMessageInterface;

// (the original calls it out of line; defined in nex_PRUDPStream.cpp)
template <>
DECOMP_NOINLINE qList<PRUDPEndPoint*>::~qList();
class EndPoint;
template <>
DECOMP_NOINLINE qMultiMap<u32, EndPoint*>::qMultiMap();

// The PRUDP stream: its end points by address and virtual port (and by principal / connection
// id), the timeouts of their packets, the signatures of the messages. The names of the members
// and of the functions without a reference name are ours.
// RTTI N2nn3nex11PRUDPStreamE @ 0x008CE050
// vtable 0x008FC334 (vptr 0x008FC33C), offset_to_top 0, 34 entries
class PRUDPStream : public ::nn::nex::ConnectionOrientedStream
{
public:
    typedef LogicalClockTmpl<u16, s16> SequenceId;
    // the results of CheckConnectionSignature
    static const int SIGNATURE_VALID = 1;
    static const int SIGNATURE_INVALID = 2;
    static const int SIGNATURE_DROPPED = 3;

    PRUDPStream(nn::nex::Stream::Type type, nn::nex::RootTransport* pTransport); // 0x0035AC5C
    virtual ~PRUDPStream(); // 0x0035AF8C slot 0x00 | slot vf_0x00 of nn::nex::Stream
    // 0x0035AF5C slot 0x04 (deleting dtor)
    virtual bool ReceiveIncomingPacket(unsigned short port, unsigned char session, nn::nex::Packet* pPacket); // 0x00359CD0 slot 0x08
    virtual void DoWork(); // 0x0035AADC slot 0x0C
    virtual bool IsDuplicateReorderingPacket(nn::nex::Packet* pPacket); // 0x0035A45C slot 0x10 | fefates:bytes-fuzzy
    virtual bool StopListen(); // 0x003589C0 slot 0x18
    virtual bool ResponsibleForURL(const nn::nex::StationURL* pURL); // 0x00359688 slot 0x1C | fefates:bytes
    virtual const wchar_t* GetURLType(); // 0x0035896C slot 0x20 | mk7dlp:bytes
    virtual nn::nex::EndPoint* OpenEndPoint(const nn::nex::StationURL* pURL); // 0x00358AC0 slot 0x24
    virtual nn::nex::EndPoint* OpenEndPoint(unsigned connectionId); // 0x00358BAC slot 0x28
    virtual bool OpenEndPoint(nn::nex::EndPoint* pEndPoint); // 0x00358B3C slot 0x2C
    virtual nn::nex::EndPoint* FindEndPointByPrincipalID(unsigned principalId); // 0x003595C8 slot 0x30
    virtual u32 GetNumberOfConnectionIDs(); // 0x00359678 slot 0x34
    virtual nn::nex::EndPoint* FindEndPointByConnectionID(unsigned connectionId); // 0x00359518 slot 0x38
    virtual nn::nex::qVector<u32> GetConnectionIDs(); // 0x0072A298 slot 0x3C
    virtual void CloseEndPoint(nn::nex::EndPoint* pEndPoint); // 0x00358BD4 slot 0x40
    virtual void Lock(); // 0x00359488 slot 0x44
    virtual void Unlock(); // 0x003596B0 slot 0x48
    virtual nn::nex::qResult Initialize(); // 0x00358980 slot 0x4C
    virtual nn::nex::qResult Terminate(); // 0x0035AC04 slot 0x50
    virtual nn::nex::qResult SendBroadcast(const nn::nex::StationURL* pURL, nn::nex::Buffer* pPayload); // 0x00358C60 slot 0x58
    virtual nn::nex::qResult SendTo(const nn::nex::StationURL* pURL, nn::nex::Buffer* pPayload); // 0x0035A7D4 slot 0x5C
    virtual bool IsReliableType(unsigned int type); // 0x0035AC20 slot 0x68
    virtual void RegisterPrincipalID(unsigned principalId, nn::nex::EndPoint* pEndPoint); // 0x0035A3F8 slot 0x70
    virtual bool UnregisterPrincipalID(unsigned principalId, nn::nex::EndPoint* pEndPoint); // 0x0035A698 slot 0x74
    virtual void RegisterConnectionID(unsigned connectionId, nn::nex::EndPoint* pEndPoint); // 0x0035A360 slot 0x78
    virtual bool UnregisterConnectionID(unsigned connectionId, nn::nex::EndPoint* pEndPoint); // 0x0035A4CC slot 0x7C
    // the connection and disconnection of all end points fail
    virtual void AbortIO(); // 0x003591F8 slot 0x80
    virtual void SetMaxSilenceTime(unsigned int milliseconds); // 0x00359A28 slot 0x84

    // checks the connection signature of a SYN/CONNECT packet (SIGNATURE_*)
    int CheckConnectionSignature(const nn::nex::InetAddress* pAddress, unsigned short port, unsigned char session, nn::nex::Packet* pPacket); // 0x00358DCC (name is ours)
    // the end point of the URL (a new one if there is none)
    nn::nex::PRUDPEndPoint* CreateEndPoint(const nn::nex::StationURL* pURL, unsigned short port, bool isOpened); // 0x00358F1C (name is ours)
    void ReleaseEndPoint(nn::nex::PRUDPEndPoint* pEndPoint); // 0x003594AC (name is ours)
    void VersionNegotiation(nn::nex::Packet* pReceived, nn::nex::Packet* pReply); // 0x003596D4 | fefates:bytes-fuzzy [tier B]
    void ReleaseEndPointImpl(nn::nex::PRUDPEndPoint* pEndPoint); // 0x003597F8 | fefates:bytes-fuzzy [tier B]
    // deletes the released end points (all, or the ones without a pending request)
    void DeleteReleasedEndPoints(bool isAll); // 0x00359B24 (name is ours)
    bool Send(unsigned short port, unsigned char session, nn::nex::PacketOut* pPacket, nn::nex::Key* pKey); // 0x0035A918 | fefates:bytes-fuzzy [tier B]
    // the interrupt callback of the core: the timeouts end at the termination time (armlink placed
    // it far from the others)
    static void InterruptCallback(void* pContext); // 0x00185FC0 (name is ours)

    TimeoutManager m_TimeoutManager;               // 0x14
    PRUDPEndPointMap m_EndPoints;                  // 0x54
    qList<PRUDPEndPoint*> m_ReleasedEndPoints;     // 0x70
    Time m_LastAckTime;                            // 0x88
    bool m_IsTerminating;                          // 0x90
    u32 m_Context;                                 // 0x94
    u16 m_DefaultPort;                             // 0x98
    TransportSignatureGenerator m_SignatureGenerator; // 0x9C
    SequenceId m_SequenceId;                       // 0xA4 (of the packets with the flags 0x600)
    qMultiMap<u32, EndPoint*> m_PrincipalIds;      // 0xAC
    qMultiMap<u32, EndPoint*> m_ConnectionIds;     // 0xC8
    qVector<u32> m_ConnectionIdList;               // 0xE4
    PRUDPMessageInterface* m_pMessageInterface;    // 0xF0
    u8 m_MessageVersion;                           // 0xF4
    CriticalSection m_Lock;                        // 0xF8
};
ASSERT_OFFSET(PRUDPStream, m_TimeoutManager, 0x14);
ASSERT_OFFSET(PRUDPStream, m_EndPoints, 0x54);
ASSERT_OFFSET(PRUDPStream, m_ReleasedEndPoints, 0x70);
ASSERT_OFFSET(PRUDPStream, m_SignatureGenerator, 0x9C);
ASSERT_OFFSET(PRUDPStream, m_PrincipalIds, 0xAC);
ASSERT_OFFSET(PRUDPStream, m_ConnectionIdList, 0xE4);
ASSERT_OFFSET(PRUDPStream, m_Lock, 0xF8);
ASSERT_SIZE(PRUDPStream, 0x120);
} // namespace nex
} // namespace nn

namespace std {
// (out of line in the original; nex_PRUDPStream.cpp)
template <>
DECOMP_NOINLINE multimap<u32, nn::nex::EndPoint*>::iterator multimap<u32, nn::nex::EndPoint*>::insert(const value_type& value);
} // namespace std
