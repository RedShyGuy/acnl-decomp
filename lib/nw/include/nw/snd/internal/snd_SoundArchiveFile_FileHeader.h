#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"
#include "nw/ut/ut_Api.h"

// the header: the binary file header and the references of the three blocks
class nw::snd::internal::SoundArchiveFile::FileHeader : public ::nw::ut::BinaryFileHeader
{
public:
    static const u32 SIGNATURE = 0x52415343; // 'CSAR'
    static const int BLOCK_COUNT = 3;

    u32 GetInfoBlockSize() const; // 0x0073FD34 | nintendogs:bytes [tier A]
    u32 GetFileBlockOffset() const; // 0x0073FD74 | nintendogs:bytes [tier A]
    u32 GetInfoBlockOffset() const; // 0x0073FDB4 | nintendogs:bytes [tier C]
    u32 GetStringBlockSize() const; // 0x0073FDF4 | nintendogs:bytes [tier A]
    u32 GetStringBlockOffset() const; // 0x0073FE2C | nintendogs:bytes [tier A]

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
ASSERT_SIZE(nw::snd::internal::SoundArchiveFile::FileHeader, 0x38);
