#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"

namespace sead {
class Heap;
} // namespace sead

namespace netgame {
// Small blocks of data (at most one byte of payload behind a four byte header) that accompany a change of
// the stage; a function of the game fills them. (All names are ours.)
class PayloadA
{
public:
    struct Header
    {
        DECOMP_NOINLINE void Set(u8 newIndex, u32 newSize); // 0x00626234
        DECOMP_NOINLINE void Copy(const Header& other); // 0x0062624C
        DECOMP_NOINLINE void Reset(); // 0x00626258

        u32 index : 8; // 0x0
        u32 size : 24;
    };
    ASSERT_SIZE(Header, 4);

    typedef s32 (*BuildFunction)(void* data, u32 player, u32 myNo, u32 size, u32 stage);
    typedef void (*ApplyFunction)(u32 sender, u32 param, const void* data, u32 size, u32 stage);
    struct Table
    {
        u32 size;
        BuildFunction build;
        ApplyFunction apply;
    };

    PayloadA(); // 0x00626904
    void Build(u32 stage); // 0x006267C4
    void Apply(u32 sender, u32 param, u32 stage) const; // 0x0075FAD4
    void Reset(); // 0x0062687C
    void Set(const void* data, u32 size); // 0x00626890
    void AllocBuffer(sead::Heap* heap); // 0x006268C8

    static const Table s_Table;

    PlayerNo m_Player; // 0x0
    u8* m_pBuffer;     // 0x4
    u32 m_Size;        // 0x8
};

class PayloadB
{
public:
    struct Header
    {
        DECOMP_NOINLINE void Set(u8 newIndex, u32 newSize); // 0x006263C0
        DECOMP_NOINLINE void Copy(const Header& other); // 0x006263D8
        DECOMP_NOINLINE void Reset(); // 0x006263E4

        u32 index : 8; // 0x0
        u32 size : 24;
    };
    ASSERT_SIZE(Header, 4);

    typedef s32 (*BuildFunction)(void* data, u32 param, u32 size, u32 stage);
    typedef void (*ApplyFunction)(u32 sender, const void* data, u32 size, u32 stage);
    struct Table
    {
        u32 size;
        BuildFunction build;
        ApplyFunction apply;
    };

    PayloadB(); // 0x00626A30
    void Build(u32 param, u32 stage); // 0x0062691C
    void Apply(u32 sender, u32 stage) const; // 0x0075FBD4
    void Reset(); // 0x006269AC
    void Set(const void* data, u32 size); // 0x006269BC
    void AllocBuffer(sead::Heap* heap); // 0x006269F4

    static const Table s_Table;

    u8* m_pBuffer; // 0x0
    u32 m_Size;    // 0x4
    u8 unk_0x8;
};

class PayloadC
{
public:
    struct Header
    {
        DECOMP_NOINLINE void Set(u8 newIndex, u32 newSize); // 0x00626A44
        DECOMP_NOINLINE void Copy(const Header& other); // 0x00626A5C
        DECOMP_NOINLINE void Reset(); // 0x00626A68

        u32 index : 8; // 0x0
        u32 size : 24;
    };
    ASSERT_SIZE(Header, 4);

    typedef s32 (*BuildFunction)(void* data, u32 param, u32 size, u32 stage);
    typedef void (*ApplyFunction)(u32 sender, const void* data, u32 size, u32 stage);
    struct Table
    {
        u32 size;
        BuildFunction build;
        ApplyFunction apply;
    };

    PayloadC(); // 0x00626FB8
    void Build(u32 param, u32 stage); // 0x00626E9C
    void Apply(u32 sender, u32 stage) const; // 0x0075FDAC
    void Reset(); // 0x00626F2C
    void Set(const void* data, u32 size); // 0x00626F44
    void AllocBuffer(sead::Heap* heap); // 0x00626F7C

    static const Table s_Table;

    u8* m_pBuffer;     // 0x0
    u32 m_Size;        // 0x4
    PlayerNo m_Player; // 0x8
    u8 unk_0x9;
};

class PayloadD
{
public:
    struct Header
    {
        DECOMP_NOINLINE void Set(u8 newIndex, u32 newSize); // 0x006263FC
        DECOMP_NOINLINE void Copy(const Header& other); // 0x00626414
        DECOMP_NOINLINE void Reset(); // 0x00626420

        u32 index : 8; // 0x0
        u32 size : 24;
    };
    ASSERT_SIZE(Header, 4);

    typedef s32 (*BuildFunction)(void* data, u32 param, u32 size, u32 stage);
    typedef void (*ApplyFunction)(u32 sender, const void* data, u32 size, u32 stage);
    struct Table
    {
        u32 size;
        BuildFunction build;
        ApplyFunction apply;
    };

    PayloadD(); // 0x00626BC8
    void Build(u32 param, u32 stage); // 0x00626AB8
    void Apply(u32 sender, u32 stage) const; // 0x0075FCC0
    void Reset(); // 0x00626B48
    void Set(const void* data, u32 size); // 0x00626B54
    void AllocBuffer(sead::Heap* heap); // 0x00626B8C

    static const Table s_Table;

    u8* m_pBuffer; // 0x0
    u32 m_Size;    // 0x4
};

class PayloadE
{
public:
    struct Header
    {
        DECOMP_NOINLINE void Set(u8 newIndex, u32 newSize); // 0x00626BD8
        DECOMP_NOINLINE void Copy(const Header& other); // 0x00626BF0
        DECOMP_NOINLINE void Reset(); // 0x00626BFC

        u32 index : 8; // 0x0
        u32 size : 24;
    };
    ASSERT_SIZE(Header, 4);

    typedef s32 (*BuildFunction)(void* data, u32 param, u32 size, u32 stage);
    typedef void (*ApplyFunction)(u32 sender, const void* data, u32 size, u32 stage);
    struct Table
    {
        u32 size;
        BuildFunction build;
        ApplyFunction apply;
    };

    PayloadE(); // 0x0062711C
    void Build(u32 param, u32 stage); // 0x0062700C
    void Apply(u32 sender, u32 stage) const; // 0x0075FE98
    void Reset(); // 0x0062709C
    void Set(const void* data, u32 size); // 0x006270A8
    void AllocBuffer(sead::Heap* heap); // 0x006270E0

    static const Table s_Table;

    u8* m_pBuffer; // 0x0
    u32 m_Size;    // 0x4
};

} // namespace netgame
