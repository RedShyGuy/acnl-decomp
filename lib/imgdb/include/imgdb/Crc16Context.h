#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_HashContextBase.h"

namespace imgdb {

// RTTI N5imgdb12Crc16ContextE
// vtable 0x00908EE0 (vptr 0x00908EE8), 9 entries
// CRC-16 with the reflected polynomial 0xA001 (the CRC of the database files); the table is made by the first
// Crc16 (all names except the class are ours)
class Crc16Context : public nn::crypto::HashContextBase
{
public:
    static const u16 POLYNOMIAL = 0xA001;

    Crc16Context() : m_IsInitialized(false), m_Crc(0) {}

    virtual void Initialize(); // 0x005A5718
    virtual void Finalize(); // 0x005A57C8
    virtual void Update(const void* data, size_t size); // 0x005A5734
    virtual void GetHash(void* output) { *static_cast<u16*>(output) = m_Crc; } // 0x005A57BC
    virtual ~Crc16Context();
    virtual size_t GetSize() const { return sizeof(m_Crc); } // 0x005A572C slot 0x20

    static void MakeTable();
    u16 GetCrc() const { return m_Crc; }

    bool m_IsInitialized;  // +0x04
    u16 m_Crc;             // +0x06

    static bool s_IsTableReady;
    static u16 s_Table[256];
};
ASSERT_SIZE(Crc16Context, 0x8);

inline void Crc16Context::Initialize()
{
    m_Crc = 0;
    m_IsInitialized = true;
}

inline void Crc16Context::Finalize()
{
    m_IsInitialized = false;
}

inline void Crc16Context::Update(const void* data, size_t size)
{
    const u8* p = static_cast<const u8*>(data);
    u16 crc = m_Crc;
    for (size_t i = 0; i < size; i++)
    {
        crc = s_Table[(p[i] ^ crc) & 0xFF] ^ (crc >> 8);
    }
    m_Crc = crc;
}

inline void Crc16Context::MakeTable()
{
    for (u32 i = 0; i < 256; i++)
    {
        u32 crc = i;
        for (int bit = 0; bit < 8; bit++)
        {
            crc = (crc & 1) != 0 ? (crc >> 1) ^ POLYNOMIAL : crc >> 1;
        }
        s_Table[i] = crc;
    }
    s_IsTableReady = true;
}

} // namespace imgdb
