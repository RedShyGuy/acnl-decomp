#pragma once

// Miis sent between consoles: a packet holds up to 128 CFLStoreData encrypted with APT:Wrap, the information keeps
// what each player sent. All names except the functions of the reference symbols are ours.

#include "cfl/cfl_Api.h"
#include "nn/Result.h"

struct CFLNetPacketHeader
{
    u32 magic;     // 0x00 "CFPB" or "CFPN"
    u32 valid[4];  // 0x04 bit per Mii
    u16 num;       // 0x14
    u16 _16;       // 0x16
};
ASSERT_SIZE(CFLNetPacketHeader, 0x18);

// a CFLStoreData after APT:Wrap (nonce and MAC added)
struct CFLNetPacketData
{
    u8 data[0x70];
};
ASSERT_SIZE(CFLNetPacketData, 0x70);

// the buffer holds a CFLNetPacketHeader and dataNum CFLStoreData for every other player
struct CFLNetInformation
{
    u8 dataNum;        // 0x00
    u8 playerNum;      // 0x01
    u16 myIndex;       // 0x02
    u32 _4[4];         // 0x04 bit per player: a packet was checked
    u32 received[4];   // 0x14 bit per player: its Miis were taken
    void* buffer;      // 0x24
};
ASSERT_SIZE(CFLNetInformation, 0x28);

struct CFLiReceivedList;
struct CFLiDevice;

extern "C" {
u32 CFL_GetNetInfoBufferSize(u32 playerNum, u32 dataNum);
s32 CFL_InitNetInformation(CFLNetInformation* info, void* buffer, u16 myIndex, u8 playerNum, u8 dataNum);
s32 CFL_InitNetPacket(CFLNetPacketHeader* packet, u16 num);
s32 CFL_SetToNetPacket(CFLNetPacketHeader* packet, u16 index, int source, const void* data, u16 dataIndex);
s32 CFL_SetReceivedPacket(CFLNetInformation* info, const CFLNetPacketHeader* packet, u16 player);
s32 CFLi_SetReceivedPacketAsync(CFLNetInformation* info, const CFLNetPacketHeader* packet, u16 player,
                                bool addToRecent);
s32 CFL_NetInfo2StoreData(CFLStoreData* store, const CFLNetInformation* info, u16 player, u16 index);
bool CFL_SearchOfficialData(u16* index, const CFLiCreateID* id);
s32 CFLi_PickupCharInfo(CFLiCharInfo* info, int source, const void* data, u16 index);
} // extern "C"

// (names are ours)
nn::Result CFLi_UnwrapNetData(void* dst, const void* src);   // 0x00157724
nn::Result CFLi_UnwrapNetData1(void* dst, const void* src);  // 0x00157740
nn::Result CFLi_WrapNetData(void* dst, const void* src);     // 0x00157AA8
nn::Result CFLi_WrapNetData1(void* dst, const void* src);    // 0x00157AC4
u32 CFL_GetNetPacketSize(u16 num);                          // 0x00153088
s32 CFLi_CheckNetPacket(CFLNetInformation* info, const CFLNetPacketHeader* packet, u16 player);  // 0x00177DF8
void CFLi_WriteRecentDatabase(CFLiReceivedList* recent);     // 0x001755F0
void CFLi_RecentDatabaseFormatted(CFLiDevice* device, s32 value);  // 0x001755CC
void CFLi_RecentDatabaseWritten(CFLiDevice* device, s32 value);    // 0x0017942C
