#include "netgame/dNetGameMgr.h"

#include "sead/seadHeap.h"

namespace netgame {
// 0x00625C78 (name is ours)
void netgame::NetGameMgr::EntryData::ClearSize(PlayerNo no)
{
    if (no < PLAYER_NO_MAX) {
        m_Sizes[no] = 0;
    }
}

// 0x00625C90 (name is ours)
void netgame::NetGameMgr::EntryData::SendToGuests(const u16* param)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
        return;
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no == 0 || !mgr->IsAdmitted(no)) {
            continue;
        }
        if (no < PLAYER_NO_MAX) {
            u8* buffer = GetSendBuffer(no);
            if (buffer != NULL) {
                *reinterpret_cast<u16*>(buffer) = *param;
                u32 total = sizeof(u16);
                u8* flag = buffer + sizeof(u16);
                for (s32 j = 0; j < SECTION_NUM; j++) {
                    SectionBuildFunction function = s_SectionBuildFunctions[j];
                    u8* data = flag + 1;
                    total++;
                    if (function == NULL) {
                        *flag = 0;
                    } else {
                        u32 size = j < SECTION_NUM ? s_SectionSizes[j] : 0;
                        bool isUsed = function(data, no, param, size);
                        if (isUsed) {
                            data += size;
                            total += size;
                        }
                        *flag = isUsed;
                    }
                    flag = data;
                }
                m_Sizes[no] = total;
            }
        }
        PacketQueue& queue = mgr->m_PacketQueue;
        queue.BeginPacket();
        if (m_Sizes[no] != 0 && no < PLAYER_NO_MAX && no != 0) {
            u8* buffer = GetSendBuffer(no);
            if (buffer != NULL) {
                queue.AddPacketData(buffer, m_Sizes[no]);
            }
        }
        queue.EndPacket(22, no);
        m_Sizes[no] = 0;
    }
}

// 0x00625E24 (name is ours)
void netgame::NetGameMgr::EntryData::Save(u32 no)
{
    if (no == 0) {
        return;
    }
    for (s32 i = 0; i < DATA_NUM; i++) {
        DataSaveFunction function = s_DataSaveFunctions[i];
        if (function != NULL && i < DATA_NUM && m_pBuffer != NULL) {
            u8* data = m_pBuffer + s_DataOffsets[i];
            if (data != NULL) {
                function(data, no, s_DataSizes[i]);
            }
        }
    }
}

// 0x00625E9C (name is ours)
u8* netgame::NetGameMgr::EntryData::GetFlag(PlayerNo no)
{
    if (no < PLAYER_NO_MAX && no != 0) {
        return m_pFlags + (no - 1);
    }
    return NULL;
}

// 0x00625EC4 (name is ours)
void netgame::NetGameMgr::EntryData::Load(u32 no)
{
    if (no == 0) {
        return;
    }
    for (s32 i = 0; i < DATA_NUM; i++) {
        DataLoadFunction function = s_DataLoadFunctions[i];
        if (function != NULL && i < DATA_NUM && m_pBuffer != NULL) {
            u8* data = m_pBuffer + s_DataOffsets[i];
            if (data != NULL) {
                function(no, data, s_DataSizes[i]);
            }
        }
    }
}

// 0x00625F34 (name is ours)
void netgame::NetGameMgr::EntryData::Receive(u32 sender, const u8* data, u32 size)
{
    if (data == NULL || size == 0) {
        return;
    }
    s32 i = 0;
    while (size <= 4) {
        bool isUsed = *data != 0;
        data++;
        size--;
        SectionApplyFunction function = s_SectionApplyFunctions[i];
        if (isUsed && function != NULL) {
            u32 sectionSize = i < SECTION_NUM ? s_SectionSizes[i] : 0;
            function(sender, data, sectionSize);
            data += sectionSize;
            size -= sectionSize;
        }
        i++;
        if (size == 0 || i >= SECTION_NUM) {
            return;
        }
    }
}

// 0x00625FD0 (name is ours)
void netgame::NetGameMgr::EntryData::BuildFlag(PlayerNo no)
{
    if (no != 0 && s_FlagBuildFunction != NULL && no < PLAYER_NO_MAX && m_pFlags != NULL) {
        u8* flag = m_pFlags + s_FlagOffset + (no - 1);
        if (flag != NULL) {
            s_FlagBuildFunction(flag, no, s_FlagSize);
        }
    }
}

// 0x0062602C (name is ours)
void netgame::NetGameMgr::EntryData::ApplyFlag(PlayerNo no)
{
    if (no != 0 && s_FlagApplyFunction != NULL && no < PLAYER_NO_MAX && m_pFlags != NULL) {
        u8* flag = m_pFlags + s_FlagOffset + (no - 1);
        if (flag != NULL) {
            s_FlagApplyFunction(no, flag, s_FlagSize);
        }
    }
}

// 0x00626090 (name is ours)
void netgame::NetGameMgr::EntryData::Reset()
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_Sizes[i] = 0;
    }
}

// 0x006260A8 (name is ours)
void netgame::NetGameMgr::EntryData::AllocBuffers(sead::Heap* heap)
{
    if (heap == NULL) {
        return;
    }
    if (m_pBuffer == NULL) {
        m_pBuffer = static_cast<u8*>(heap->tryAlloc(BUFFER_SIZE, 4));
    }
    if (m_pFlags == NULL) {
        m_pFlags = static_cast<u8*>(heap->tryAlloc(3, 4));
    }
    if (m_pSendBuffers == NULL) {
        m_pSendBuffers = static_cast<u8*>(heap->tryAlloc(18, 4));
    }
}

// 0x0062613C (name is ours)
netgame::NetGameMgr::EntryData::EntryData()
{
    m_pBuffer = NULL;
    m_pFlags = NULL;
    m_pSendBuffers = NULL;
}
} // namespace netgame
