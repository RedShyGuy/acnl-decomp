#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
// The building blocks of the sound files (BCSAR, BCWAR, BCWAV, ...; the names are ours): typed
// references to other parts by offset, tables of references and tables of values.

// a part of the file: its type and its offset from the base the user knows (-1: none)
struct BinaryReference
{
    static const s32 INVALID_OFFSET = -1;

    u16 typeId;   // 0x0
    u16 padding;  // 0x2
    s32 offset;   // 0x4

    // (inline)
    bool IsValid() const { return offset != INVALID_OFFSET; }
};
ASSERT_SIZE(BinaryReference, 0x8);

// a reference with the size of the part (the blocks of a file)
struct BinaryReferenceWithSize : public BinaryReference
{
    u32 size; // 0x8
};
ASSERT_SIZE(BinaryReferenceWithSize, 0xC);

// a table of references; the offsets count from the table
struct BinaryReferenceTable
{
    u32 count;                  // 0x0
    BinaryReference items[1];   // 0x4

    // (inline) the part of an item (NULL: beyond the table)
    const void* GetReferedItem(u32 index) const
    {
        if (index >= count) {
            return nullptr;
        }
        return reinterpret_cast<const u8*>(this) + items[index].offset;
    }
    // (inline) the part of an item if it has the type (NULL: beyond the table or another type)
    const void* GetReferedItem(u32 index, u16 typeId) const
    {
        if (index >= count || items[index].typeId != typeId) {
            return nullptr;
        }
        return reinterpret_cast<const u8*>(this) + items[index].offset;
    }
};

// a table of values
template <typename T>
struct BinaryTable
{
    u32 count; // 0x0
    T items[1]; // 0x4
};

// (inline) the part a reference points to from a base (no check of the type)
inline const void* GetReferedPointer(const void* pBase, const BinaryReference& reference)
{
    return static_cast<const u8*>(pBase) + reference.offset;
}
// (inline) the block of a type of a file whose header lists dataBlocks blocks after the binary file
// header (NULL: none or at offset 0)
inline const void* GetBlockByType(const void* pHeader, u16 dataBlocks, const BinaryReferenceWithSize* pBlocks, u16 typeId)
{
    for (int i = 0; i < dataBlocks; ++i) {
        const BinaryReferenceWithSize* pReference = &pBlocks[i];
        if (pReference->typeId == typeId) {
            if (pReference != nullptr && pReference->offset != 0) {
                return GetReferedPointer(pHeader, *pReference);
            }
            return nullptr;
        }
    }
    return nullptr;
}

// the envelope of a note: attack, decay, sustain, hold and release, 0 to 127 (name is ours)
struct AdshrCurve
{
    // (inline) all 0 (the callers of WaveSoundFileReader::ReadNoteInfo clear the envelope)
    AdshrCurve() : attack(0), decay(0), sustain(0), hold(0), release(0) {}
    AdshrCurve(u8 attack, u8 decay, u8 sustain, u8 hold, u8 release)
        : attack(attack), decay(decay), sustain(sustain), hold(hold), release(release)
    {
    }

    u8 attack;  // 0x0
    u8 decay;   // 0x1
    u8 sustain; // 0x2
    u8 hold;    // 0x3
    u8 release; // 0x4
};
ASSERT_SIZE(AdshrCurve, 0x5);

// a wave of the banks and wave sounds: the wave archive (item id) and the index in it (name is ours)
struct WaveId
{
    u32 waveArchiveId; // 0x0
    u32 waveIndex;     // 0x4 (-1: none)
};
ASSERT_SIZE(WaveId, 0x8);
} // namespace internal
} // namespace snd
} // namespace nw
