#include "nw/font/font_ResFont.h"
#include "nw/font/internal/font_TextureObject.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace font {
namespace {
// the offset is made a pointer (0 stays NULL; inline, name is ours)
template <typename T>
inline void ResolveOffset(T*& rPtr, void* pBase)
{
    rPtr = reinterpret_cast<T*>(reinterpret_cast<u8*>(pBase) + reinterpret_cast<uptr>(rPtr));
}
template <typename T>
inline void ResolveOffsetIfNotZero(T*& rPtr, void* pBase)
{
    if (rPtr != 0) {
        ResolveOffset(rPtr, pBase);
    }
}
// the data behind a block header
inline void* GetBlockData(ut::BinaryBlockHeader* pBlock)
{
    return reinterpret_cast<u8*>(pBlock) + sizeof(ut::BinaryBlockHeader);
}
inline ut::BinaryBlockHeader* GetFirstBlock(ut::BinaryFileHeader* pHeader)
{
    return reinterpret_cast<ut::BinaryBlockHeader*>(reinterpret_cast<u8*>(pHeader) + pHeader->headerSize);
}
inline ut::BinaryBlockHeader* GetNextBlock(ut::BinaryBlockHeader* pBlock)
{
    return reinterpret_cast<ut::BinaryBlockHeader*>(reinterpret_cast<u8*>(pBlock) + pBlock->size);
}
} // namespace

// 0x004D95D8 (name is ours)
nw::font::ResFont::ResFont()
{
    // (only the base and the vtable)
}

// 0x001386B0 slot 0x00
// 0x004D95F0 (deleting dtor)
nw::font::ResFont::~ResFont()
{
    if (m_pResource != nullptr) {
        RemoveResource();
    }
}

// 0x00138698 (name is ours)
void* nw::font::ResFont::ReleaseResource()
{
    if (m_pResource == nullptr) {
        return nullptr;
    }
    return RemoveResource();
}

// 0x0013C4B8 (name is ours)
DECOMP_NOINLINE void* nw::font::ResFont::RemoveResource()
{
    if (m_pTextureObjects != nullptr) {
        DeleteTextureNames();
        m_pTextureObjects = nullptr;
    }
    void* pResource = m_pResource;
    m_pResource = nullptr;
    m_pFontInfo = nullptr;
    return pResource;
}

// 0x004D9368 (name is ours)
bool nw::font::ResFont::SetResource(void* pBfnt)
{
    if (m_pResource != nullptr) {
        return false;
    }
    ut::BinaryFileHeader* pHeader = static_cast<ut::BinaryFileHeader*>(pBfnt);
    FontInformation* pFontInfo = nullptr;
    if (pHeader->signature == SIGNATURE_UNPACKED) {
        // relocated already: only the font information is needed
        ut::BinaryBlockHeader* pBlock = GetFirstBlock(pHeader);
        for (int i = 0; i < pHeader->dataBlocks; i++) {
            if (pBlock->kind == BLOCK_FINF) {
                pFontInfo = static_cast<FontInformation*>(GetBlockData(pBlock));
                break;
            }
            pBlock = GetNextBlock(pBlock);
        }
    } else {
        if (!ut::IsValidBinaryFile(pHeader, SIGNATURE, VERSION, MIN_BLOCKS)) {
            return false;
        }
        ut::BinaryBlockHeader* pBlock = GetFirstBlock(pHeader);
        for (int i = 0; i < pHeader->dataBlocks; i++) {
            switch (pBlock->kind) {
            case BLOCK_FINF: {
                pFontInfo = static_cast<FontInformation*>(GetBlockData(pBlock));
                ResolveOffset(pFontInfo->pGlyph, pHeader);
                ResolveOffsetIfNotZero(pFontInfo->pWidth, pHeader);
                ResolveOffsetIfNotZero(pFontInfo->pMap, pHeader);
                break;
            }
            case BLOCK_TGLP: {
                FontTextureGlyph* pGlyph = static_cast<FontTextureGlyph*>(GetBlockData(pBlock));
                ResolveOffset(pGlyph->sheetImage, pHeader);
                break;
            }
            case BLOCK_CWDH: {
                FontWidth* pWidth = static_cast<FontWidth*>(GetBlockData(pBlock));
                ResolveOffsetIfNotZero(pWidth->pNext, pHeader);
                break;
            }
            case BLOCK_CMAP: {
                FontCodeMap* pMap = static_cast<FontCodeMap*>(GetBlockData(pBlock));
                ResolveOffsetIfNotZero(pMap->pNext, pHeader);
                break;
            }
            case BLOCK_GLGR:
                break;
            default:
                return false;
            }
            pBlock = GetNextBlock(pBlock);
        }
        pHeader->signature = SIGNATURE_UNPACKED;
    }
    if (pFontInfo == nullptr) {
        return false;
    }
    SetResourceBuffer(pBfnt, pFontInfo);
    return true;
}

// 0x0013C52C | nintendogs:bytes [tier A]
void* nw::font::ResFont::SetDrawBuffer(void* pBuffer)
{
    void* pOldBuffer = m_pTextureObjects;
    if (pOldBuffer == pBuffer) {
        return pBuffer;
    }
    if (pOldBuffer != nullptr) {
        DeleteTextureNames();
    }
    m_pTextureObjects = static_cast<internal::TextureObject*>(pBuffer);
    if (pBuffer != nullptr) {
        GenTextureNames();
    }
    return pOldBuffer;
}

// 0x004D9540 | nintendogs:bytes [tier A]
u32 nw::font::ResFont::GetDrawBufferSize(const void* pBfnt)
{
    ut::BinaryFileHeader* pHeader = static_cast<ut::BinaryFileHeader*>(const_cast<void*>(pBfnt));
    if (pHeader->signature != SIGNATURE_UNPACKED && !ut::IsValidBinaryFile(pHeader, SIGNATURE, VERSION, MIN_BLOCKS)) {
        return 0;
    }
    ut::BinaryBlockHeader* pBlock = GetFirstBlock(pHeader);
    for (int i = 0; i < pHeader->dataBlocks; i++) {
        if (pBlock->kind == BLOCK_TGLP) {
            const FontTextureGlyph* pGlyph = static_cast<FontTextureGlyph*>(GetBlockData(pBlock));
            if (pGlyph == nullptr) {
                break;
            }
            return pGlyph->sheetNum * sizeof(internal::TextureObject);
        }
        pBlock = GetNextBlock(pBlock);
    }
    return 0;
}

} // namespace font
} // namespace nw
