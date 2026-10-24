#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveArchiveFile.h"

class nw::snd::internal::WaveArchiveFile::FileHeader : public ::nw::ut::BinaryFileHeader
{
public:
    static const int BLOCK_COUNT = 2;

    const void* GetFileBlock() const; // 0x0013FAC4 | nintendogs:bytes [tier A]
    const void* GetInfoBlock() const; // 0x0013FAF8 | nintendogs:bytes [tier A]
    u32 GetFileBlockOffset() const; // 0x00141D28 | nintendogs:bytes [tier A]
    u32 GetInfoBlockOffset() const; // 0x00143A90 | nintendogs:bytes [tier A]

    // (inline) the reference of a block (NULL: none)
    const BinaryReferenceWithSize* GetReferenceBy(u16 typeId) const
    {
        for (int i = 0; i < BLOCK_COUNT; ++i) {
            if (blocks[i].typeId == typeId) {
                return &blocks[i];
            }
        }
        return nullptr;
    }

    BinaryReferenceWithSize blocks[BLOCK_COUNT]; // 0x14
};
