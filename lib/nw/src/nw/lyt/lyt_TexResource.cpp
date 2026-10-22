#include "nw/lyt/lyt_TexResource.h"

namespace nw {
namespace lyt {
// 0x004B4EA4 | nintendogs:bytes [tier B]
bool nw::lyt::TexResource::Set(void* pData, u32 size)
{
    // the image data is 128 byte aligned, the file header follows it (its offset is the last word)
    if (pData != nullptr && (reinterpret_cast<uptr>(pData) & 0x7F) == 0 && (size & 3) == 0 && size > sizeof(ut::BinaryFileHeader) + sizeof(res::Image) + sizeof(u32)) {
        const u32* pFileSize = reinterpret_cast<const u32*>(reinterpret_cast<uptr>(pData) + size) - 1;
        if (size > *pFileSize) {
            ut::BinaryFileHeader* pFileHeader = reinterpret_cast<ut::BinaryFileHeader*>(reinterpret_cast<uptr>(pData) + ((*pFileSize + 3) & ~3));
            if (ut::IsValidBinaryFile(pFileHeader, res::FILE_SIGNATURE_IMAGE, res::FILE_VERSION, 1)) {
                const res::Image* pImage = nullptr;
                ut::BinaryBlockHeader* pBlock = nullptr;
                for (int i = 0; i < pFileHeader->dataBlocks; ++i) {
                    pBlock = ut::GetNextBinaryBlockHeader(pFileHeader, pBlock);
                    if (pBlock->kind == res::BLOCK_KIND_IMAGE) {
                        pImage = reinterpret_cast<const res::Image*>(pBlock);
                    }
                }
                if (pImage != nullptr) {
                    m_pData = pData;
                    m_pImage = pImage;
                    m_pFileSize = pFileSize;
                    return true;
                }
            }
        }
    }

    m_pData = nullptr;
    m_pImage = nullptr;
    m_pFileSize = nullptr;
    return false;
}

} // namespace lyt
} // namespace nw
