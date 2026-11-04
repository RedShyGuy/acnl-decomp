#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"

namespace sead {
class Heap;
} // namespace sead

namespace netgame {
// The packets of a frame in one buffer (each with a 6 byte header) and the state packets: a block of data
// per id that is kept up to date on all stations; a change marks the id for sending. (Names after libgarden:
// PacketQueue, BeginPacket, AddPacketData, EndPacket, m_pBuffer, m_pReceiveStateBuffer, m_pSendStateBuffer,
// m_SendStateBufferSize, m_pShouldSendStateUpdate, m_LeavingPlayer, m_Counter, m_Counter2, m_Tick, m_IsClosed;
// the others are ours.)
class PacketQueue
{
public:
    static const u32 BUFFER_SIZE = 0x16800;
    static const u32 STATE_BUFFER_SIZE = 0x813;
    static const u32 STATE_NUM = 150;
    static const u32 PACKET_NUM = 133;
    static const u32 SYSTEM_PACKET_NUM = 27;

    typedef void (*StateInitFunction)(void* state);
    // writes the new value into the copy of the state
    typedef void (*StateUpdateFunction)(void* state, const void* value);
    typedef void (*PacketHandler)(u32 sender, const void* data, u32 size, u32 target, u32 stage);
    typedef void (*SystemPacketHandler)(u32 sender, const void* data, u32 size, u32 kind);

    // the header in front of a packet
    struct PacketHeader
    {
        u32 GetSize() const { return sizeLow | (sizeMid << 8) | (sizeHigh << 16); }

        u8 id;           // 0x0
        u8 sizeLow;      // 0x1
        u8 sizeMid;      // 0x2
        u8 sizeHigh : 2; // 0x3
        u8 target : 3;
        u8 stage;        // 0x4
        u8 sender : 2;   // 0x5
        u8 unknown : 1;
    };
    ASSERT_SIZE(PacketHeader, 6);

    PacketQueue(); // 0x00628FF0
    void AllocBuffers(sead::Heap* heap); // 0x00628EF0
    void Reset(); // 0x00628E24

    DECOMP_NOINLINE void BeginPacket(); // 0x00628A4C | libgarden
    void AddPacketData(const void* data, size_t size); // 0x0062881C | libgarden
    void EndPacket(u8 id, PlayerNo target); // 0x00628AF4 | libgarden

    // passes value to the state id; if that changes the state it is stored and shouldSend is noted for sending
    void UpdateState(u32 id, const void* value, bool shouldSend); // 0x006288AC
    void ClearState(u32 id); // 0x00628CF4
    void ClearAllStates(); // 0x00628D98
    void ClearStateRequests(); // 0x00628D80

    void IncrementCounter(PlayerNo player); // 0x00628AB8
    void ResetCounters(PlayerNo player); // 0x00628AD8
    void ResetCounter(PlayerNo player); // 0x00628BCC

    // the packets of a received block to their handlers
    static void DispatchPackets(const void* data, u32 size); // 0x00628C20
    static s8 GetSystemPacketKind(u32 kind); // 0x00628BE4
    static void DispatchSystemPacket(u32 sender, const void* data, u32 size, u32 kind); // 0x00628BFC

    // the packets of the other stations of the last frame
    void Process(); // 0x00627904
    void FinishFrame();
    // sends the packets of the frame
    void ProcessEnd(); // 0x00627BFC
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetCounter(PlayerNo player) const // 0x0075FFF8
    {
        if (player < PLAYER_NO_MAX) {
            return m_Counter[player];
        }
        return 0;
    }
    // the state of the id (NULL before the buffers exist)
    DECOMP_EMIT_COPY DECOMP_NOINLINE void* GetState(u32 id) const // 0x0076000C
    {
        if (id < STATE_NUM && m_pReceiveStateBuffer != NULL) {
            return m_pReceiveStateBuffer + s_StateOffsets[id];
        }
        return NULL;
    }

    bool IsClosed() const { return m_IsClosed; }

    static const u32 s_StateBufferSize;
    static const s8 s_SystemPacketKinds[SYSTEM_PACKET_NUM];
    static const u32 s_StateSizes[STATE_NUM];
    static const u32 s_StateOffsets[STATE_NUM];
    static const StateInitFunction s_StateInitFunctions[STATE_NUM];
    static const PacketHandler s_PacketHandlers[PACKET_NUM];
    static const StateUpdateFunction s_StateUpdateFunctions[STATE_NUM];
    static const SystemPacketHandler s_SystemPacketHandlers[SYSTEM_PACKET_NUM];

    u8* m_pBuffer;                  // 0x00
    u8* m_pFront;                   // 0x04 the packet being built
    u8* m_pBack;                    // 0x08 its end
    u8* m_pReceiveStateBuffer;      // 0x0C
    u8* m_pSendStateBuffer;         // 0x10
    size_t m_SendStateBufferSize;   // 0x14
    bool* m_pShouldSendStateUpdate; // 0x18
    s32 m_Counter[4];               // 0x1C
    PlayerNo m_LeavingPlayer;       // 0x2C
    s32 m_Counter2[4];              // 0x30
    s32 m_Tick;                     // 0x40
    bool m_IsClosed;                // 0x44
};
ASSERT_SIZE(PacketQueue, 0x48);
} // namespace netgame
