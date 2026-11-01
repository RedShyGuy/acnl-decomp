#include "cfl/cfl_Database.h"
#include "cfl/cfl_Net.h"
#include "cfl/cfl_RecentDB.h"
#include "nn/applet/CTR/detail/detail_Api.h"

#include <cstring>

namespace {
// the magic of the packets ("CFPN" with the second key of APT:Wrap1, else "CFPB"; names are ours)
const u32 MAGIC_CFPN = 0x4E504643;
const u32 MAGIC_CFPB = 0x42504643;

inline bool TestBit(const u32* bits, int index)
{
    return (bits[index / 32] & (1 << (index % 32))) != 0;
}

// the block of a player in the buffer of the information; the own player has none (inline; name is ours)
inline u8* GetPlayerBlock(const CFLNetInformation* info, u16 player)
{
    u32 size = sizeof(CFLNetPacketHeader) + info->dataNum * sizeof(CFLStoreData);
    u16 index = player;
    if (info->myIndex < player) {
        index = player - 1;
    }
    return static_cast<u8*>(info->buffer) + size * (info->myIndex != player ? index : 0);
}

// marks an asynchronous operation (inline; name is ours)
inline void BeginWorking()
{
    if (CFL_IsAvailable()) {
        CFLi_GetManager()->working = true;
        CFLi_GetManager()->status = CFL_RESULT_BUSY;
    }
}
} // namespace

// the encryption of the Miis of a packet with APT:Wrap/Unwrap (names are ours)

// 0x00157724 (name is ours)
DECOMP_NOINLINE nn::Result CFLi_UnwrapNetData(void* dst, const void* src)
{
    return nn::applet::CTR::detail::Unwrap(dst, src, sizeof(CFLNetPacketData), 12, 10);
}

// 0x00157740 (name is ours)
DECOMP_NOINLINE nn::Result CFLi_UnwrapNetData1(void* dst, const void* src)
{
    return nn::applet::CTR::detail::Unwrap1(dst, src, sizeof(CFLNetPacketData), 12, 10);
}

// 0x00157AA8 (name is ours)
DECOMP_NOINLINE nn::Result CFLi_WrapNetData(void* dst, const void* src)
{
    return nn::applet::CTR::detail::Wrap(dst, src, sizeof(CFLStoreData), 12, 10);
}

// 0x00157AC4 (name is ours)
DECOMP_NOINLINE nn::Result CFLi_WrapNetData1(void* dst, const void* src)
{
    return nn::applet::CTR::detail::Wrap1(dst, src, sizeof(CFLStoreData), 12, 10);
}

// 0x00153088 (name is ours)
DECOMP_NOINLINE u32 CFL_GetNetPacketSize(u16 num)
{
    return sizeof(CFLNetPacketHeader) + num * sizeof(CFLNetPacketData);
}

// checks a received packet and copies its Miis into the block of the player
// 0x00177DF8 (name is ours)
DECOMP_NOINLINE s32 CFLi_CheckNetPacket(CFLNetInformation* info, const CFLNetPacketHeader* packet, u16 player)
{
    s32 status = CFL_RESULT_INVALID_ARGUMENT;
    u32 magic = s_NetMagicN ? MAGIC_CFPN : MAGIC_CFPB;
    if (packet->magic != magic) {
        return CFL_RESULT_INVALID_DATA;
    }
    u16 index = info->myIndex < player ? static_cast<u16>(player - 1) : player;
    if (info->myIndex == player || info->playerNum - 1 <= index) {
        return status;
    }
    status = CFL_RESULT_SUCCESS;
    info->received[player / 32] &= ~(1 << (player % 32));
    u8* block = GetPlayerBlock(info, player);
    const CFLNetPacketData* entries = reinterpret_cast<const CFLNetPacketData*>(packet + 1);
    for (int i = 0; i < packet->num && i < info->dataNum; i++) {
        if (!TestBit(packet->valid, i)) {
            continue;
        }
        CFLStoreData* store = reinterpret_cast<CFLStoreData*>(block + sizeof(CFLNetPacketHeader)) + i;
        if (s_NetMagicN) {
            CFLi_UnwrapNetData1(store, &entries[i]);
        } else {
            CFLi_UnwrapNetData(store, &entries[i]);
        }
        if (CFLi_GetCRC16(0, store, sizeof(CFLStoreData)) != 0) {
            return CFL_RESULT_INVALID_DATA;
        }
        CFLiCharInfo info2;
        CFLi_UnpackMiiDataCore(&info2, &store->official.core, true, store->official.creatorName);
        if (CFLi_IsNullMiiID(&info2.createID) || CFLi_IsTemporaryMiiID(&info2.createID) ||
            !CFLi_VerifyCharInfo(&info2, true)) {
            return CFL_RESULT_INVALID_DATA;
        }
    }
    *reinterpret_cast<CFLNetPacketHeader*>(block) = *packet;
    info->_4[player / 32] |= 1 << (player % 32);
    return status;
}

// writes the Miis of the database that are new into the recent list (name is ours)
// 0x001755F0 (name is ours)
DECOMP_NOINLINE void CFLi_WriteRecentDatabase(CFLiReceivedList* recent)
{
    if (recent->num == 0) {
        CFLi_EndWorkingReason(CFL_RESULT_SUCCESS, 0);
        return;
    }
    recent->callback = CFLi_RecentDatabaseWritten;
    if (s_pDatabase == NULL || CFLi_GetFileBroken(1) || CFLi_GetFileBroken(0)) {
        CFLi_FormatDatabase(CFLi_RecentDatabaseFormatted);
        return;
    }
    CFLiDevice* device = s_pDatabaseDevice;
    device->create = true;
    device->flush = true;
    CFLi_SequenceOpen(device, CFLi_StartWriteDatabase);
}

// 0x001755CC (name is ours)
void CFLi_RecentDatabaseFormatted(CFLiDevice*, s32)
{
    CFLiDevice* device = s_pDatabaseDevice;
    device->create = true;
    device->flush = true;
    CFLi_SequenceOpen(device, CFLi_StartWriteDatabase);
}

// 0x0017942C (name is ours)
void CFLi_RecentDatabaseWritten(CFLiDevice* device, s32)
{
    if (CFL_GetAsyncStatus() == CFL_RESULT_FILE_ERROR) {
        CFLi_GetManager()->status = CFL_RESULT_5;
    }
    device->state = 0;
    device->callback = NULL;
    device->offset = 0;
    device->buffer = NULL;
    device->size = 0;
    device->step = CFLi_SequenceCloseStep;
    device->_10 = false;
    CFLi_SequenceCloseStep(device, 0);
}

extern "C" {
// 0x00153070 | nintendogs:bytes [tier B]
u32 CFL_GetNetInfoBufferSize(u32 playerNum, u32 dataNum)
{
    return (playerNum - 1) * (sizeof(CFLNetPacketHeader) + dataNum * sizeof(CFLStoreData));
}

// 0x00153550 | nintendogs:bytes [tier B]
s32 CFL_InitNetInformation(CFLNetInformation* info, void* buffer, u16 myIndex, u8 playerNum, u8 dataNum)
{
    if (info == NULL || buffer == NULL || playerNum <= myIndex) {
        return CFL_RESULT_INVALID_ARGUMENT;
    }
    if (static_cast<u32>(playerNum - 2) > 126 || (playerNum - 1) * dataNum > 128) {
        return CFL_RESULT_INVALID_ARGUMENT;
    }
    std::memset(info, 0, offsetof(CFLNetInformation, buffer));
    info->dataNum = dataNum;
    info->playerNum = playerNum;
    info->myIndex = myIndex;
    info->buffer = buffer;
    return CFL_RESULT_SUCCESS;
}

// 0x001535D0 | nintendogs:callseq [confirmed by mk7dlp] [tier A]
s32 CFL_InitNetPacket(CFLNetPacketHeader* packet, u16 num)
{
    if (packet == NULL || static_cast<u32>(num - 1) > 127) {
        return CFL_RESULT_INVALID_ARGUMENT;
    }
    if (!CFL_IsAvailable()) {
        return CFL_RESULT_NOT_AVAILABLE;
    }
    std::memset(packet, 0, CFL_GetNetPacketSize(num));
    packet->magic = s_NetMagicN ? MAGIC_CFPN : MAGIC_CFPB;
    packet->valid[0] = 0;
    packet->valid[1] = 0;
    packet->valid[2] = 0;
    packet->valid[3] = 0;
    packet->_16 = 0;
    packet->num = num;
    return CFL_RESULT_SUCCESS;
}

// puts a Mii into a packet
// 0x001540B0 | nintendogs:callseq [confirmed by mk7dlp] [tier A]
s32 CFL_SetToNetPacket(CFLNetPacketHeader* packet, u16 index, int source, const void* data, u16 dataIndex)
{
    if (packet == NULL || packet->num <= index) {
        return CFL_RESULT_INVALID_ARGUMENT;
    }
    if (source != 0 && source != 2 && source != 4 && source != 5) {
        return CFL_RESULT_INVALID_ARGUMENT;
    }
    u32* word = &packet->valid[index / 32];
    u32 bit = 1 << (index % 32);
    *word &= ~bit;
    if (!CFL_IsAvailable()) {
        return CFL_RESULT_NOT_AVAILABLE;
    }
    CFLiCharInfo info;
    s32 status = CFLi_PickupCharInfo(&info, source, data, dataIndex);
    if (status != CFL_RESULT_SUCCESS) {
        return status;
    }
    CFLiMiiDataOfficial official;
    CFLi_PackMiiDataCore(&official.core, official.creatorName, &info, true);
    official.core.favorite = 0;
    CFLStoreData store;
    if (&store.official != &official) {
        store.official = official;
    }
    store._5C = 0;
    CFLi_PutCRC16(0, &store, sizeof(store));
    CFLNetPacketData* entries = reinterpret_cast<CFLNetPacketData*>(packet + 1);
    if (s_NetMagicN) {
        CFLi_WrapNetData1(&entries[index], &store);
    } else {
        CFLi_WrapNetData(&entries[index], &store);
    }
    *word |= bit;
    return CFL_RESULT_SUCCESS;
}

// 0x0015401C | nintendogs:bytes [confirmed by mk7dlp] [tier A]
s32 CFL_SetReceivedPacket(CFLNetInformation* info, const CFLNetPacketHeader* packet, u16 player)
{
    s32 status = CFLi_SetReceivedPacketAsync(info, packet, player, true);
    if (status == CFL_RESULT_BUSY) {
        return CFL_WaitAsync();
    }
    return status;
}

// takes the Miis of a received packet; with addToRecent the new ones go into the recent list of the database
// 0x001569E4 | nintendogs:bytes-fuzzy [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE s32 CFLi_SetReceivedPacketAsync(CFLNetInformation* info, const CFLNetPacketHeader* packet, u16 player,
                                bool addToRecent)
{
    if (info == NULL || packet == NULL) {
        return CFL_RESULT_INVALID_ARGUMENT;
    }
    if (!CFL_IsAvailable() || (CFL_IsAvailable() && CFLi_GetManager()->working)) {
        return CFL_RESULT_NOT_AVAILABLE;
    }
    s32 status = CFLi_CheckNetPacket(info, packet, player);
    if (status != CFL_RESULT_SUCCESS) {
        return status;
    }
    BeginWorking();
    if (!CFL_IsAvailable() || s_pDatabase == NULL || CFLi_GetFileBroken(1) || CFLi_GetFileBroken(0) ||
        s_pDatabase->official._4_8 || !addToRecent) {
        info->received[player / 32] |= 1 << (player % 32);
        CFLi_EndWorkingReason(CFL_RESULT_SUCCESS, 0);
        return CFL_GetAsyncStatus();
    }
    CFLNetPacketHeader* block = reinterpret_cast<CFLNetPacketHeader*>(GetPlayerBlock(info, player));
    CFLiReceivedList* recent = s_pReceivedList;
    recent->callback = NULL;
    recent->num = 0;
    recent->moveToEnd = true;
    for (int i = 0; i < block->num; i++) {
        if (!TestBit(block->valid, i)) {
            continue;
        }
        CFLStoreData* store = reinterpret_cast<CFLStoreData*>(block + 1) + i;
        if (!CFLi_IsValidStoreDataInner(store) || !CFLi_IsValidMiiID(&store->official.core.createID) ||
            store->official.core.localOnly) {
            continue;
        }
        if (CFL_SearchOfficialData(NULL, &store->official.core.createID)) {
            continue;
        }
        CFLiHiddenEntry entry;
        if (&entry.core != &store->official.core) {
            entry.core = store->official.core;
        }
        entry.core.birthMonth = 0;
        entry.core.birthDay = 0;
        int year, month, day, hour, minute, second;
        CFLi_GetNowDateTime(&year, &month, &day, &hour, &minute, &second);
        year -= 2000;
        if (year < 0) {
            year = 0;
        } else if (year > 127) {
            year = 127;
        }
        entry.timestamp = (year << 25) | (month << 21) | (day << 16) | (hour << 11) | (minute << 5) | (second / 2);
        entry._4C = 0;
        entry._50 = 0;
        CFLiHiddenEntry* dst = &recent->entries[recent->num];
        std::memcpy(dst, &entry, sizeof(entry));
        dst->core.favorite = 0;
        recent->num++;
    }
    CFLi_WriteRecentDatabase(recent);
    CFL_GetAsyncStatus();
    info->received[player / 32] |= 1 << (player % 32);
    return CFL_GetAsyncStatus();
}

// copies a received Mii
// 0x00153D0C | nintendogs:bytes [confirmed by mk7dlp] [tier A]
s32 CFL_NetInfo2StoreData(CFLStoreData* store, const CFLNetInformation* info, u16 player, u16 index)
{
    if (store == NULL || info == NULL) {
        return CFL_RESULT_INVALID_ARGUMENT;
    }
    if (!CFL_IsAvailable()) {
        return CFL_RESULT_NOT_AVAILABLE;
    }
    if (!TestBit(info->received, player)) {
        return CFL_RESULT_NOT_FOUND;
    }
    const CFLNetPacketHeader* block = reinterpret_cast<const CFLNetPacketHeader*>(GetPlayerBlock(info, player));
    const CFLStoreData* src = reinterpret_cast<const CFLStoreData*>(block + 1) + index;
    if (!TestBit(block->valid, index) || !CFLi_IsValidStoreDataInner(src)) {
        return CFL_RESULT_NOT_FOUND;
    }
    std::memcpy(store, src, sizeof(CFLStoreData));
    return CFL_RESULT_SUCCESS;
}
} // extern "C"
