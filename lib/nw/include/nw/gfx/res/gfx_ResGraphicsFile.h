#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/os/os_IAllocator.h"

namespace nw {
namespace gfx {
namespace res {
// A graphics file (3dbrew "CGFX"): the header, then the DATA block with a dictionary per kind of
// object (field names after the 3dbrew page).
struct ResGraphicsFileData
{
    u32 signature;           // 0x00 "CGFX"
    u16 byteOrder;           // 0x04
    u16 headerSize;          // 0x06
    u32 revision;            // 0x08
    u32 fileSize;            // 0x0C
    u32 blockCount;          // 0x10
    u32 dataSignature;       // 0x14 "DATA"
    u32 dataSize;            // 0x18
    ResDicInfo models;       // 0x1C
    ResDicInfo textures;     // 0x24
    ResDicInfo luts;         // 0x2C
    ResDicInfo materials;    // 0x34
    ResDicInfo shaders;      // 0x3C
    ResDicInfo cameras;      // 0x44
    ResDicInfo lights;       // 0x4C
    ResDicInfo fogs;         // 0x54
    ResDicInfo environments; // 0x5C
    ResDicInfo skeletalAnims; // 0x64
    ResDicInfo textureAnims; // 0x6C
    ResDicInfo visibilityAnims; // 0x74
    ResDicInfo cameraAnims;  // 0x7C
    ResDicInfo lightAnims;   // 0x84
    ResDicInfo emitters;     // 0x8C
};
ASSERT_OFFSET(ResGraphicsFileData, emitters, 0x8C);

// the binary resource of a graphics file (the wrapper of its data; the class name and Setup /
// Cleanup are from the reference symbols)
class ResGraphicsFile
{
public:
    // (inline)
    explicit ResGraphicsFile(const void* pData = nullptr) : m_pData(static_cast<const ResGraphicsFileData*>(pData)) {}
    const ResGraphicsFileData* ptr() const { return m_pData; }
    bool IsValid() const { return m_pData != nullptr; }

    void Cleanup(); // 0x00131E5C | nintendogs:bytes [tier A]
    // the objects of the file: lookup tables, lights, fogs, shaders, textures, animations, models
    u32 Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile); // 0x004A6D94 | nintendogs:callseq [tier A]

    const ResGraphicsFileData* m_pData; // 0x0
};
ASSERT_SIZE(ResGraphicsFile, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
