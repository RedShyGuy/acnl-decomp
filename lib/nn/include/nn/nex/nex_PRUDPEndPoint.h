#pragma once

#include "decomp.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_IORequestContext.h"
#include "nn/nex/nex_LogicalClockTmpl.h"
#include "nn/nex/nex_PacketEncDec.h"
#include "nn/nex/nex_ProfilingUnit.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"
#include "nn/nex/nex_RTT.h"
#include "nn/nex/nex_SignatureBytes.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qMap.h"
#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class Packet;
class PacketDispatchQueue;
class PacketIn;
class PacketOut;
class PRUDPMessageInterface;
class PRUDPStream;
class SlidingWindow;

// the version of the PRUDP functions of the end points
extern PseudoGlobalVariable<u8> g_PRUDPFunctionsVersion;
// the supported functions above the version
extern u32 g_PRUDPSupportedFunctions;
// the time of a synchronous connection or disconnection while the core terminates
extern s32 g_TerminationTimeout;

// The statistics of a PRUDP end point (EndPoint::GetStatistics). The names are ours.
struct PRUDPEndPointStatistics
{
    // (at 0x0035C67C) clears the counters
    void Reset();

    u32 m_Unknown0x00;                 // 0x00 (not reset)
    u32 m_Unknown0x04;                 // 0x04 (not reset)
    Time m_ResetTime;                  // 0x08
    u32 m_ConnectionId;                // 0x10 (GetStatistics)
    u8 m_StreamId;                     // 0x14 ("sid" of the URL)
    u8 m_SessionId;                    // 0x15
    u8 m_PeerSessionId;                // 0x16
    u8 m_FaultReason;                  // 0x17
    u32 m_LastRTT;                     // 0x18
    u32 m_RTO;                         // 0x1C
    u32 m_SentPackets;                 // 0x20
    u32 m_SentUnreliablePackets;       // 0x24
    u32 m_SentUnreliableDataPackets;   // 0x28
    u32 m_SentPackets0x200;            // 0x2C (with the flag 0x200)
    u32 m_SentPackets0x400;            // 0x30 (with the flag 0x400)
    u32 m_SentPings;                   // 0x34
    u32 m_Unknown0x38;                 // 0x38
    u32 m_DelayedDisconnects;          // 0x3C (the reliable packets were not sent yet)
    u32 m_TooLargeSends;               // 0x40
    u32 m_SentBytes;                   // 0x44
    u32 m_SentUnreliableBytes;         // 0x48
    u32 m_SentUnreliableDataBytes;     // 0x4C
    u32 m_SentBytes0x200;              // 0x50
    u32 m_SentBytes0x400;              // 0x54
    u32 m_ReceivedPackets;             // 0x58
    u32 m_ReceivedUnreliablePackets;   // 0x5C
    u32 m_ReceivedPackets0x200;        // 0x60
    u32 m_ReceivedPackets0x400;        // 0x64
    u32 m_ReceivedBytes;               // 0x68
    u32 m_ReceivedUnreliableBytes;     // 0x6C
    u32 m_ReceivedBytes0x200;          // 0x70
    u32 m_ReceivedBytes0x400;          // 0x74
    u32 m_UnexpectedPongs;             // 0x78
    u32 m_SentAcks;                    // 0x7C
    u32 m_UnknownAcks;                 // 0x80 (of no queued packet)
    u32 m_LostUnreliablePackets;       // 0x84
    u32 m_Unknown0x88;                 // 0x88
    u32 m_DuplicatePackets;            // 0x8C
    u32 m_SentReliablePackets;         // 0x90
    u32 m_SentReliableBytes;           // 0x94
    u32 m_FragmentedSends;             // 0x98
    u32 m_SendFailures;                // 0x9C
    u32 m_Retransmissions;             // 0xA0
    u32 m_RetransmittedBytes;          // 0xA4
    u32 m_MaxRetransmissions;          // 0xA8
    u32 m_FullWindows;                 // 0xAC
    u32 m_Unknown0xB0;                 // 0xB0
    u32 m_ReceivedReliablePackets;     // 0xB4
    u32 m_ReceivedReliableBytes;       // 0xB8
    u32 m_MaxRTO;                      // 0xBC
    u32 m_MinRTO;                      // 0xC0 (-1: none yet)
    u32 m_LastPingSequenceId;          // 0xC4
    u32 m_LastPongSequenceId;          // 0xC8
};
ASSERT_OFFSET(PRUDPEndPointStatistics, m_SentPackets, 0x20);
ASSERT_OFFSET(PRUDPEndPointStatistics, m_MaxRTO, 0xBC);
ASSERT_SIZE(PRUDPEndPointStatistics, 0xD0);

// The PRUDP connection to a station: the reliable substreams (sliding windows, dispatch queues,
// fragments), the unreliable sequences, the pings, the connection and the disconnection, the
// encryption and the statistics. The names of the members and of the functions without a
// reference name are ours.
// RTTI N2nn3nex13PRUDPEndPointE @ 0x008CE248
// vtable 0x008FC828 (vptr 0x008FC830), offset_to_top 0, 37 entries
class PRUDPEndPoint : public ::nn::nex::EndPoint
{
public:
    typedef LogicalClockTmpl<u16, s16> SequenceId;

    // the flags of the type/flags word of the packets (the names are ours)
    static const u16 FLAG_ACK = 0x10;
    static const u16 FLAG_RELIABLE = 0x20;
    static const u16 FLAG_NEED_ACK = 0x40;
    static const u16 FLAG_HAS_SIZE = 0x80;
    static const u16 FLAG_UNKNOWN_0x200 = 0x200;
    static const u16 FLAG_UNKNOWN_0x400 = 0x400;
    static const u16 FLAG_MULTI_ACK = 0x2000;

    PRUDPEndPoint(nn::nex::PRUDPStream* pStream, const nn::nex::StationURL* pURL, nn::nex::LogicalClockTmpl<unsigned short, short>* pStreamSequenceId, unsigned char messageVersion); // 0x003682D4 (name is ours)
    virtual ~PRUDPEndPoint(); // 0x003689C8 slot 0x00 | slot vf_0x00 of nn::nex::EndPoint
    // 0x00368998 slot 0x04 (deleting dtor)
    virtual void ResetStatistics(); // 0x003636AC slot 0x08
    virtual bool GetStatistics(void* pStatistics); // 0x0072A65C slot 0x0C
    virtual u32 GetPendingReliablePacketCount(); // 0x0072A8E0 slot 0x10
    virtual void GetPendingReliablePacketCounts(nn::nex::qVector<u32>* pCounts); // 0x0072A8F0 slot 0x14
    virtual u16 GetMaxSubstreamId(); // 0x0072A8B0 slot 0x18
    virtual bool IsNotConnected(); // 0x00362EB4 slot 0x1C | fefates:bytes
    virtual bool IsConnecting(); // 0x00362C18 slot 0x20 | slot vf_0x20 of nn::nex::EndPoint
    virtual bool IsDisconnecting(); // 0x00362FE0 slot 0x24 | fefates:bytes
    virtual bool IsFaulty(); // 0x00367CF0 slot 0x28 | fefates:bytes
    virtual bool IsConnected(); // 0x00362730 slot 0x2C | fefates:bytes
    virtual bool PeerIsConnected(); // 0x00362FF4 slot 0x30 | slot vf_0x30 of nn::nex::EndPoint
    virtual bool PeerIsNotConnected(); // 0x00363984 slot 0x34
    virtual void SetKeepAliveTimeout(unsigned int milliseconds); // 0x003646BC slot 0x3C | slot vf_0x3C of nn::nex::EndPoint
    virtual void SetMaxSilenceTime(unsigned int milliseconds); // 0x003636F0 slot 0x40 | slot vf_0x40 of nn::nex::EndPoint
    virtual u32 GetKeepAliveTimeout(); // 0x00363A88 slot 0x44 | slot vf_0x44 of nn::nex::EndPoint
    virtual u32 GetMaxSilenceTime(); // 0x00363248 slot 0x48 | slot vf_0x48 of nn::nex::EndPoint
    virtual void SetPeerConnected(); // 0x00363200 slot 0x4C | slot vf_0x4C of nn::nex::EndPoint
    virtual void SetPeerDisconnected(); // 0x00364718 slot 0x50 | slot vf_0x50 of nn::nex::EndPoint
    virtual u8 GetConnectionState() const; // 0x0072A8C0 slot 0x54 | slot vf_0x54 of nn::nex::EndPoint
    virtual bool SetConnectionState(nn::nex::EndPoint::_ConnectionState state); // 0x00363998 slot 0x58 | fefates:bytes-fuzzy
    virtual bool IsAnyPacketProcessed(); // 0x00364724 slot 0x60 | fefates:bytes
    virtual u32 GetSignaledFaultError() const; // 0x0072A8C8 slot 0x64 | slot vf_0x64 of nn::nex::EndPoint
    virtual u32 GetLastRTT(); // 0x0072A98C slot 0x68
    virtual u32 GetRTO(); // 0x0072A650 slot 0x6C
    virtual nn::nex::qResult _Connect(nn::nex::Buffer* pConnectData, nn::nex::Buffer* pReplyData, CompletionCallback pCallback, const nn::nex::UserContext& context, unsigned int timeout); // 0x00367DA8 slot 0x70 | fefates:bytes-fuzzy
    virtual nn::nex::qResult _Disconnect(CompletionCallback pCallback, const nn::nex::UserContext& context, unsigned int timeout); // 0x00362744 slot 0x74 | slot vf_0x74 of nn::nex::EndPoint
    virtual nn::nex::qResult _Send(nn::nex::Buffer* pBuffer, unsigned int flags, bool copy, unsigned char substreamId, unsigned int id); // 0x003670DC slot 0x78 | slot vf_0x78 of nn::nex::EndPoint
    virtual u64 GetSupportedFunctionsFlags() const; // 0x0072A8D0 slot 0x7C | slot vf_0x7C of nn::nex::EndPoint
    virtual void SignalFaultEvent(unsigned int error, bool isProcessed); // 0x0036320C slot 0x88 | fefates:bytes
    virtual void SetEncryptionKey(const nn::nex::Key& key); // 0x0035D984 slot 0x8C | fefates:bytes
    virtual const nn::nex::Key& GetEncryptionKey(); // 0x00362FFC slot 0x90 | slot vf_0x90 of nn::nex::EndPoint

    // completes the connection or disconnection with the result
    bool CompleteIO(nn::nex::qResult result); // 0x003620B0 | fefates:bytes [tier B]
    nn::nex::qResult SendPacket(nn::nex::PacketOut* pPacket); // 0x003621AC | fefates:bytes-fuzzy [tier B]
    // the silence of the peer is checked, a ping is sent when it is time
    void TimeToPing(); // 0x00362558
    // sends a reliable packet (again) and schedules its retransmission
    void TransmitPacket(nn::nex::PacketOut* pPacket); // 0x00362C28 (name is ours)
    // passes received data to the event handler
    void DispatchData(nn::nex::Buffer* pData, nn::nex::PacketIn* pPacket); // 0x00362EC8 (name is ours)
    // acknowledges with the payload (several sequence ids)
    void SendAck(nn::nex::Buffer* pPayload, unsigned short sequenceId, bool isNewFormat); // 0x00363004 (name is ours)
    void SendAggregateACK(); // 0x0036311C | fefates:bytes [tier B]
    void InitUnreliableSeq(nn::nex::PacketOut* pPacket); // 0x00363250 | fefates:bytes [tier B]
    void ProcessFaultEvent(unsigned int error); // 0x003632DC
    // the reliable packet of the substream was acknowledged
    void ProcessAck(nn::nex::Time time, nn::nex::LogicalClockTmpl<unsigned short, short> sequenceId, unsigned char substreamId); // 0x003636F8 (name is ours)
    void SendAggregateACKNew(); // 0x00363A90
    void SendAggregateACKOld(); // 0x003641EC
    // a packet of the peer
    void ProcessPacket(nn::nex::PacketIn* pPacket); // 0x00364780 (name is ours)
    bool IsDuplicateReorderingPacket(nn::nex::PacketIn* pPacket); // 0x003656A8 | fefates:bytes [tier B]
    // the window of the substream is full (the connection fails)
    nn::nex::qResult ProcessFullWindow(); // 0x00365794 (name is ours)
    void ProcessAggregateAckNew(nn::nex::PacketIn* pPacket); // 0x00365900 (name is ours)
    void ProcessAggregateAckOld(nn::nex::PacketIn* pPacket); // 0x00365E24 (name is ours)
    void DecideMaxUserReliableSubStreams(unsigned short maxSubstreamId); // 0x00366288
    // sends the payload in fragments of fragmentSize bytes (in buffers of bufferSize bytes)
    nn::nex::qResult SendFragmented(nn::nex::Buffer* pPayload, unsigned int bufferSize, unsigned int fragmentSize, unsigned int type, unsigned short flags, unsigned char unknown0x130,
                                    unsigned int id, unsigned char substreamId); // 0x00366C08 (name is ours)
    // the payload of the fragments of a reliable packet so far; true when it is complete
    bool Defrag(nn::nex::PacketIn* pPacket); // 0x00367718 | fefates:bytes [tier B]
    // acknowledges the packet
    void AcknowledgePacket(nn::nex::PacketIn* pPacket); // 0x00367904 (name is ours)
    // decodes and dispatches a received unreliable packet (the time is not used)
    void DispatchUnreliablePacket(nn::nex::PacketIn* pPacket, nn::nex::Time time); // 0x00367C1C (name is ours)
    void StopPing(); // 0x00367D04 (name is ours)
    // the round trip time of an acknowledged packet
    void UpdateRTT(nn::nex::Time time, nn::nex::PacketOut* pPacket); // 0x0036813C (name is ours)
    void StartPing(); // 0x00368204 | fefates:bytes [tier B]

    // (inline) the stream of the end point
    PRUDPStream* GetStream() const { return reinterpret_cast<PRUDPStream*>(m_pStream); }

    u16 m_PeerPort;                                  // 0x08E (the virtual port of the peer)
    qVector<PacketDispatchQueue*> m_DispatchQueues;  // 0x090 (per reliable substream)
    qVector<SlidingWindow*> m_SlidingWindows;        // 0x09C
    PacketOut* m_pPingPacket;                        // 0x0A8
    PacketOut* m_pSynPacket;                         // 0x0AC
    bool m_IsConnectSent;                            // 0x0B0
    u32 m_MaxSilenceTime;                            // 0x0B4
    u32 m_KeepAliveTimeout;                          // 0x0B8
    u8 m_ConnectionState;                            // 0x0BC
    bool m_IsPeerConnected;                          // 0x0BD
    u32 m_FaultEvent;                                // 0x0C0
    RTT m_RTT;                                       // 0x0C4
    Time m_LastSendTime;                             // 0x0D8
    Time m_LastReceiveTime;                          // 0x0E0
    SequenceId m_UnreliableSequenceId;               // 0x0E8
    SequenceId m_PeerUnreliableSequenceId;           // 0x0F0
    SequenceId* m_pStreamSequenceId;                 // 0x0F8 (of the packets with the flags 0x600)
    SequenceId m_PeerStreamSequenceId;               // 0x0FC
    u16 m_MaxSubstreamId;                            // 0x104
    SignatureBytes m_Signature;                      // 0x108 (the connection signature of the end point)
    SignatureBytes m_PeerSignature;                  // 0x11C
    qVector<Buffer*> m_Fragments;                    // 0x130 (per substream)
    qVector<u8> m_FragmentIds;                       // 0x13C
    u8 m_PeerSessionId;                              // 0x148
    u8 m_SessionId;                                  // 0x149
    u8 m_FaultReason;                                // 0x14A
    u8 m_PeerStreamSessionId;                        // 0x14B
    u8 m_MessageVersion;                             // 0x14C
    u32 m_SupportedFunctions;                        // 0x150
    u64 m_PeerSupportedFunctions;                    // 0x158
    u8 m_Unknown0x160;                               // 0x160
    std::vector<bool, MemAllocator<bool> > m_PendingAcks; // 0x164 (per substream)
    ProfilingUnit m_SendProfiling;                   // 0x178
    IORequestContext m_IORequest;                    // 0x1E8 (the connection or disconnection)
    PRUDPMessageInterface* m_pMessageInterface;      // 0x1FC
    PacketEncDec m_EncDec;                           // 0x200
    bool m_IgnoreNextFault;                          // 0x26C
    PRUDPEndPointStatistics m_Statistics;            // 0x270
    qMap<u32, u32> m_FragmentIdCounts;               // 0x340
};
ASSERT_OFFSET(PRUDPEndPoint, m_DispatchQueues, 0x90);
ASSERT_OFFSET(PRUDPEndPoint, m_RTT, 0xC4);
ASSERT_OFFSET(PRUDPEndPoint, m_MaxSubstreamId, 0x104);
ASSERT_OFFSET(PRUDPEndPoint, m_PeerSessionId, 0x148);
ASSERT_OFFSET(PRUDPEndPoint, m_PendingAcks, 0x164);
ASSERT_OFFSET(PRUDPEndPoint, m_SendProfiling, 0x178);
ASSERT_OFFSET(PRUDPEndPoint, m_IORequest, 0x1E8);
ASSERT_OFFSET(PRUDPEndPoint, m_EncDec, 0x200);
ASSERT_OFFSET(PRUDPEndPoint, m_Statistics, 0x270);
ASSERT_OFFSET(PRUDPEndPoint, m_FragmentIdCounts, 0x340);
ASSERT_SIZE(PRUDPEndPoint, 0x360);
} // namespace nex
} // namespace nn
