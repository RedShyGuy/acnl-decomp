// (generated from the common pattern of the five classes)
#include "netgame/dNetGamePayload.h"

#include "netgame/dNetGameMgr.h"
#include "sead/seadHeap.h"
#include <cstring>

namespace netgame {
// 0x00626234 (name is ours)
void netgame::PayloadA::Header::Set(u8 newIndex, u32 newSize)
{
    index = newIndex;
    size = newSize;
}

// 0x0062624C (name is ours)
void netgame::PayloadA::Header::Copy(const Header& other)
{
    *this = other;
}

// 0x00626258 (name is ours)
void netgame::PayloadA::Header::Reset()
{
    index = 0;
    size = 0;
}

// 0x006267C4 (name is ours)
void netgame::PayloadA::Build(u32 stage)
{
    u8* buffer = m_pBuffer;
    if (buffer == NULL) {
        return;
    }
    PlayerNo player = m_Player;
    if (player >= PLAYER_NO_MAX) {
        return;
    }
    u32 total = 0;
    PlayerNo myNo = NetGameMgr::s_pInstance->GetMyPlayerNo();
    BuildFunction function = s_Table.build;
    u32 maxSize = s_Table.size;
    if (function != NULL) {
        s32 size = function(buffer + sizeof(Header), player, myNo, maxSize, stage);
        if (size >= 0 && static_cast<u32>(size) <= maxSize) {
            Header header;
            header.Reset();
            header.Set(0, size);
            *reinterpret_cast<Header*>(buffer) = header;
            total = size + sizeof(Header);
        }
    }
    m_Size = total;
}

// 0x0075FAD4 (name is ours)
void netgame::PayloadA::Apply(u32 sender, u32 param, u32 stage) const
{
    u32 left = m_Size;
    if (left > 5) {
        return;
    }
    const u8* data = m_pBuffer;
    if (data == NULL) {
        return;
    }
    while (left >= sizeof(Header)) {
        Header raw = *reinterpret_cast<const Header*>(data);
        data += sizeof(Header);
        left -= sizeof(Header);
        Header header;
        header.Reset();
        header.Copy(raw);
        ApplyFunction function = s_Table.apply;
        if (header.index == 0 && function != NULL) {
            u32 size = header.size;
            if (size > s_Table.size || size > left) {
                return;
            }
            if (size != 0) {
                function(sender, param, data, size, stage);
                data += size;
                // (the remaining size grows: also in the original)
                left += size;
            } else {
                function(sender, param, NULL, 0, stage);
            }
        }
    }
}

// 0x0062687C (name is ours)
void netgame::PayloadA::Reset()
{
    m_Size = 0;
    m_Player = PLAYER_NO_INVALID;
}

// 0x00626890 (name is ours)
void netgame::PayloadA::Set(const void* data, u32 size)
{
    if (data == NULL || size > 5 || m_pBuffer == NULL) {
        return;
    }
    if (size != 0) {
        std::memcpy(m_pBuffer, data, size);
    }
    m_Size = size;
}

// 0x006268C8 (name is ours)
void netgame::PayloadA::AllocBuffer(sead::Heap* heap)
{
    if (heap != NULL && m_pBuffer == NULL) {
        m_pBuffer = static_cast<u8*>(heap->tryAlloc(5, 4));
    }
}

// 0x00626904 (name is ours)
netgame::PayloadA::PayloadA()
{
    m_Player = PLAYER_NO_INVALID;
    m_pBuffer = NULL;
    m_Size = 0;
}

// 0x006263C0 (name is ours)
void netgame::PayloadB::Header::Set(u8 newIndex, u32 newSize)
{
    index = newIndex;
    size = newSize;
}

// 0x006263D8 (name is ours)
void netgame::PayloadB::Header::Copy(const Header& other)
{
    *this = other;
}

// 0x006263E4 (name is ours)
void netgame::PayloadB::Header::Reset()
{
    index = 0;
    size = 0;
}

// 0x0062691C (name is ours)
void netgame::PayloadB::Build(u32 param, u32 stage)
{
    u8* buffer = m_pBuffer;
    if (buffer == NULL) {
        return;
    }
    u32 total = 0;
    BuildFunction function = s_Table.build;
    u32 maxSize = s_Table.size;
    if (function != NULL) {
        s32 size = function(buffer + sizeof(Header), param, maxSize, stage);
        if (size >= 0 && static_cast<u32>(size) <= maxSize) {
            Header header;
            header.Reset();
            header.Set(0, size);
            *reinterpret_cast<Header*>(buffer) = header;
            total = size + sizeof(Header);
        }
    }
    m_Size = total;
}

// 0x0075FBD4 (name is ours)
void netgame::PayloadB::Apply(u32 sender, u32 stage) const
{
    u32 left = m_Size;
    if (left > 5) {
        return;
    }
    const u8* data = m_pBuffer;
    if (data == NULL) {
        return;
    }
    while (left >= sizeof(Header)) {
        Header raw = *reinterpret_cast<const Header*>(data);
        data += sizeof(Header);
        left -= sizeof(Header);
        Header header;
        header.Reset();
        header.Copy(raw);
        ApplyFunction function = s_Table.apply;
        if (header.index == 0 && function != NULL) {
            u32 size = header.size;
            if (size > s_Table.size || size > left) {
                return;
            }
            if (size != 0) {
                function(sender, data, size, stage);
                data += size;
                // (the remaining size grows: also in the original)
                left += size;
            } else {
                function(sender, NULL, 0, stage);
            }
        }
    }
}

// 0x006269AC (name is ours)
void netgame::PayloadB::Reset()
{
    m_Size = 0;
    unk_0x8 = 0;
}

// 0x006269BC (name is ours)
void netgame::PayloadB::Set(const void* data, u32 size)
{
    if (data == NULL || size > 5 || m_pBuffer == NULL) {
        return;
    }
    if (size != 0) {
        std::memcpy(m_pBuffer, data, size);
    }
    m_Size = size;
}

// 0x006269F4 (name is ours)
void netgame::PayloadB::AllocBuffer(sead::Heap* heap)
{
    if (heap != NULL && m_pBuffer == NULL) {
        m_pBuffer = static_cast<u8*>(heap->tryAlloc(5, 4));
    }
}

// 0x00626A30 (name is ours)
netgame::PayloadB::PayloadB()
{
    m_pBuffer = NULL;
    m_Size = 0;
    unk_0x8 = 0;
}

// 0x00626A44 (name is ours)
void netgame::PayloadC::Header::Set(u8 newIndex, u32 newSize)
{
    index = newIndex;
    size = newSize;
}

// 0x00626A5C (name is ours)
void netgame::PayloadC::Header::Copy(const Header& other)
{
    *this = other;
}

// 0x00626A68 (name is ours)
void netgame::PayloadC::Header::Reset()
{
    index = 0;
    size = 0;
}

// 0x00626E9C (name is ours)
void netgame::PayloadC::Build(u32 param, u32 stage)
{
    u8* buffer = m_pBuffer;
    if (buffer == NULL) {
        return;
    }
    u32 total = 0;
    BuildFunction function = s_Table.build;
    u32 maxSize = s_Table.size;
    if (function != NULL) {
        s32 size = function(buffer + sizeof(Header), param, maxSize, stage);
        if (size >= 0 && static_cast<u32>(size) <= maxSize) {
            Header header;
            header.Reset();
            header.Set(0, size);
            *reinterpret_cast<Header*>(buffer) = header;
            total = size + sizeof(Header);
        }
    }
    m_Size = total;
}

// 0x0075FDAC (name is ours)
void netgame::PayloadC::Apply(u32 sender, u32 stage) const
{
    u32 left = m_Size;
    if (left > 5) {
        return;
    }
    const u8* data = m_pBuffer;
    if (data == NULL) {
        return;
    }
    while (left >= sizeof(Header)) {
        Header raw = *reinterpret_cast<const Header*>(data);
        data += sizeof(Header);
        left -= sizeof(Header);
        Header header;
        header.Reset();
        header.Copy(raw);
        ApplyFunction function = s_Table.apply;
        if (header.index == 0 && function != NULL) {
            u32 size = header.size;
            if (size > s_Table.size || size > left) {
                return;
            }
            if (size != 0) {
                function(sender, data, size, stage);
                data += size;
                // (the remaining size grows: also in the original)
                left += size;
            } else {
                function(sender, NULL, 0, stage);
            }
        }
    }
}

// 0x00626F2C (name is ours)
void netgame::PayloadC::Reset()
{
    m_Size = 0;
    m_Player = PLAYER_NO_INVALID;
    unk_0x9 = 0;
}

// 0x00626F44 (name is ours)
void netgame::PayloadC::Set(const void* data, u32 size)
{
    if (data == NULL || size > 5 || m_pBuffer == NULL) {
        return;
    }
    if (size != 0) {
        std::memcpy(m_pBuffer, data, size);
    }
    m_Size = size;
}

// 0x00626F7C (name is ours)
void netgame::PayloadC::AllocBuffer(sead::Heap* heap)
{
    if (heap != NULL && m_pBuffer == NULL) {
        m_pBuffer = static_cast<u8*>(heap->tryAlloc(5, 4));
    }
}

// 0x00626FB8 (name is ours)
netgame::PayloadC::PayloadC()
{
    m_pBuffer = NULL;
    m_Size = 0;
    m_Player = PLAYER_NO_INVALID;
    unk_0x9 = 0;
}

// 0x006263FC (name is ours)
void netgame::PayloadD::Header::Set(u8 newIndex, u32 newSize)
{
    index = newIndex;
    size = newSize;
}

// 0x00626414 (name is ours)
void netgame::PayloadD::Header::Copy(const Header& other)
{
    *this = other;
}

// 0x00626420 (name is ours)
void netgame::PayloadD::Header::Reset()
{
    index = 0;
    size = 0;
}

// 0x00626AB8 (name is ours)
void netgame::PayloadD::Build(u32 param, u32 stage)
{
    u8* buffer = m_pBuffer;
    if (buffer == NULL) {
        return;
    }
    u32 total = 0;
    BuildFunction function = s_Table.build;
    u32 maxSize = s_Table.size;
    if (function != NULL) {
        s32 size = function(buffer + sizeof(Header), param, maxSize, stage);
        if (size >= 0 && static_cast<u32>(size) <= maxSize) {
            Header header;
            header.Reset();
            header.Set(0, size);
            *reinterpret_cast<Header*>(buffer) = header;
            total = size + sizeof(Header);
        }
    }
    m_Size = total;
}

// 0x0075FCC0 (name is ours)
void netgame::PayloadD::Apply(u32 sender, u32 stage) const
{
    u32 left = m_Size;
    if (left > 5) {
        return;
    }
    const u8* data = m_pBuffer;
    if (data == NULL) {
        return;
    }
    while (left >= sizeof(Header)) {
        Header raw = *reinterpret_cast<const Header*>(data);
        data += sizeof(Header);
        left -= sizeof(Header);
        Header header;
        header.Reset();
        header.Copy(raw);
        ApplyFunction function = s_Table.apply;
        if (header.index == 0 && function != NULL) {
            u32 size = header.size;
            if (size > s_Table.size || size > left) {
                return;
            }
            if (size != 0) {
                function(sender, data, size, stage);
                data += size;
                // (the remaining size grows: also in the original)
                left += size;
            } else {
                function(sender, NULL, 0, stage);
            }
        }
    }
}

// 0x00626B48 (name is ours)
void netgame::PayloadD::Reset()
{
    m_Size = 0;
}

// 0x00626B54 (name is ours)
void netgame::PayloadD::Set(const void* data, u32 size)
{
    if (data == NULL || size > 5 || m_pBuffer == NULL) {
        return;
    }
    if (size != 0) {
        std::memcpy(m_pBuffer, data, size);
    }
    m_Size = size;
}

// 0x00626B8C (name is ours)
void netgame::PayloadD::AllocBuffer(sead::Heap* heap)
{
    if (heap != NULL && m_pBuffer == NULL) {
        m_pBuffer = static_cast<u8*>(heap->tryAlloc(5, 4));
    }
}

// 0x00626BC8 (name is ours)
netgame::PayloadD::PayloadD()
{
    m_pBuffer = NULL;
    m_Size = 0;
}

// 0x00626BD8 (name is ours)
void netgame::PayloadE::Header::Set(u8 newIndex, u32 newSize)
{
    index = newIndex;
    size = newSize;
}

// 0x00626BF0 (name is ours)
void netgame::PayloadE::Header::Copy(const Header& other)
{
    *this = other;
}

// 0x00626BFC (name is ours)
void netgame::PayloadE::Header::Reset()
{
    index = 0;
    size = 0;
}

// 0x0062700C (name is ours)
void netgame::PayloadE::Build(u32 param, u32 stage)
{
    u8* buffer = m_pBuffer;
    if (buffer == NULL) {
        return;
    }
    u32 total = 0;
    BuildFunction function = s_Table.build;
    u32 maxSize = s_Table.size;
    if (function != NULL) {
        s32 size = function(buffer + sizeof(Header), param, maxSize, stage);
        if (size >= 0 && static_cast<u32>(size) <= maxSize) {
            Header header;
            header.Reset();
            header.Set(0, size);
            *reinterpret_cast<Header*>(buffer) = header;
            total = size + sizeof(Header);
        }
    }
    m_Size = total;
}

// 0x0075FE98 (name is ours)
void netgame::PayloadE::Apply(u32 sender, u32 stage) const
{
    u32 left = m_Size;
    if (left > 5) {
        return;
    }
    const u8* data = m_pBuffer;
    if (data == NULL) {
        return;
    }
    while (left >= sizeof(Header)) {
        Header raw = *reinterpret_cast<const Header*>(data);
        data += sizeof(Header);
        left -= sizeof(Header);
        Header header;
        header.Reset();
        header.Copy(raw);
        ApplyFunction function = s_Table.apply;
        if (header.index == 0 && function != NULL) {
            u32 size = header.size;
            if (size > s_Table.size || size > left) {
                return;
            }
            if (size != 0) {
                function(sender, data, size, stage);
                data += size;
                // (the remaining size grows: also in the original)
                left += size;
            } else {
                function(sender, NULL, 0, stage);
            }
        }
    }
}

// 0x0062709C (name is ours)
void netgame::PayloadE::Reset()
{
    m_Size = 0;
}

// 0x006270A8 (name is ours)
void netgame::PayloadE::Set(const void* data, u32 size)
{
    if (data == NULL || size > 5 || m_pBuffer == NULL) {
        return;
    }
    if (size != 0) {
        std::memcpy(m_pBuffer, data, size);
    }
    m_Size = size;
}

// 0x006270E0 (name is ours)
void netgame::PayloadE::AllocBuffer(sead::Heap* heap)
{
    if (heap != NULL && m_pBuffer == NULL) {
        m_pBuffer = static_cast<u8*>(heap->tryAlloc(5, 4));
    }
}

// 0x0062711C (name is ours)
netgame::PayloadE::PayloadE()
{
    m_pBuffer = NULL;
    m_Size = 0;
}

} // namespace netgame
