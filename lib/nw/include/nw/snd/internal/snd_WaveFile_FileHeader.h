#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveFile.h"

class nw::snd::internal::WaveFile::FileHeader : public ::nw::ut::BinaryFileHeader
{
public:
    // the blocks (NULL: none)
    const void* GetDataBlock() const; // 0x007430CC | nintendogs:bytes [tier C]
    const void* GetInfoBlock() const; // 0x00743134 | nintendogs:bytes [tier A]

    // (inline) the block of a type (NULL: none)
    const void* GetBlock(u16 typeId) const
    {
        for (int i = 0; i < dataBlocks; ++i) {
            const BinaryReferenceWithSize* pReference = &blocks[i];
            if (pReference->typeId == typeId) {
                if (pReference != nullptr && pReference->offset != 0) {
                    return GetReferedPointer(this, *pReference);
                }
                return nullptr;
            }
        }
        return nullptr;
    }

    BinaryReferenceWithSize blocks[2]; // 0x14
};
