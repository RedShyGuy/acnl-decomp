#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_Types.h"

namespace nn {
namespace gr {
namespace CTR {
class BindSymbol;

// A shader binary (DVLB) with its vertex and optional geometry shader, and the command buffers
// built from it. The binary layout follows 3dbrew "SHBIN"; all member names are ours.
class Shader
{
public:
    static const s32 EXE_COUNT_MAX = 32;

    Shader(); // 0x0034ABD0 | fefates:bytes [tier B]

    void SetupBinary(const void* binary, int vsIndex, int gsIndex); // 0x00349E64 | nintendogs:callseq [confirmed by fefates, mk7dlp] [tier A]
    u32* MakeFullCommand(u32* command) const; // 0x0072770C | nintendogs:bytes-fuzzy [confirmed by fefates, mk7dlp] [tier A]
    bool SearchBindSymbol(BindSymbol* symbol, const char* name) const; // 0x007279E4 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]

    // switches the geometry shader off (name is ours)
    static u32* MakeDisableCommand(u32* command); // 0x0034A930 (name is ours)

private:
    // DVLE header
    struct ExeHeader
    {
        u32 magic;                  // 0x00
        u16 version;                // 0x04
        u8 shaderType;              // 0x06, 0 vertex, 1 geometry
        u8 mergeOutmaps;            // 0x07
        u32 mainOffset;             // 0x08
        u32 endmainOffset;          // 0x0C
        u16 inputMask;              // 0x10
        u16 outputMask;             // 0x12
        u8 geometryType;            // 0x14
        u8 geometryStartRegister;   // 0x15
        u8 geometryFullVertexCount; // 0x16
        u8 geometryVertexCount;     // 0x17
        u32 constTableOffset;       // 0x18
        u32 constCount;             // 0x1C
        u32 labelTableOffset;       // 0x20
        u32 labelCount;             // 0x24
        u32 outputTableOffset;      // 0x28
        u32 outputCount;            // 0x2C
        u32 uniformTableOffset;     // 0x30
        u32 uniformCount;           // 0x34
        u32 symbolTableOffset;      // 0x38
        u32 symbolTableSize;        // 0x3C
    };

    // DVLP header
    struct ProgramHeader
    {
        u32 magic;          // 0x00
        u32 version;        // 0x04
        u32 codeOffset;     // 0x08
        u32 codeCount;      // 0x0C
        u32 swizzleOffset;  // 0x10
        u32 swizzleCount;   // 0x14
    };

    struct OutputEntry
    {
        u16 type;           // 0 position, 1 quaternion, 2 color, 3 texcoord0, 4 texcoord0 w,
                            // 5 texcoord1, 6 texcoord2, 8 view
        u16 registerIndex;
        u16 mask;
        u16 padding;
    };

    struct ConstEntry
    {
        u16 type; // 0 bool, 1 integer vector, 2 float24 vector
        u16 registerIndex;
        u32 value[4];
    };

    struct UniformEntry
    {
        u32 symbolOffset;
        u16 startRegister;
        u16 endRegister;
    };

    template <typename T>
    static const T* At(const void* base, u32 offset)
    {
        return reinterpret_cast<const T*>(static_cast<const u8*>(base) + offset);
    }

    DECOMP_NOINLINE static u32* MakeShaderModeCommand_(u32* command, bool isGeometryShader, PicaDataDrawMode drawMode); // 0x0034A93C | nintendogs:bytes [confirmed by fefates] [tier A]
    DECOMP_NOINLINE u32* MakeOutAttrCommand_(u32* command, int vsIndex, int gsIndex); // 0x00349F50 | nintendogs:callseq [confirmed by fefates] [tier A]
    DECOMP_NOINLINE void MakeShaderConstCommandCache_(); // 0x0034AA2C | nintendogs:bytes [confirmed by fefates] [tier A]
    DECOMP_NOINLINE u32* MakeLoadCommand_(u32* command, u32 reg, const u32* data, u32 count) const; // 0x00727944 | nintendogs:bytes [confirmed by fefates] [tier A]

    s32 m_VsIndex;                           // 0x0000
    s32 m_GsIndex;                           // 0x0004, -1 without geometry shader
    u8 m_ExeCount;                           // 0x0008
    const ExeHeader* m_Exes[EXE_COUNT_MAX];  // 0x000C
    const u32* m_Instructions;               // 0x008C
    u32 m_InstructionCount;                  // 0x0090
    u32 m_Swizzles[128];                     // 0x0094
    u32 m_SwizzleCount;                      // 0x0294
    PicaDataDrawMode m_DrawMode;             // 0x0298
    u32 m_VsBoolMask;                        // 0x029C
    u32 m_GsBoolMask;                        // 0x02A0
    u32 m_OutAttrCommand[48];                // 0x02A4
    u32 m_OutAttrCommandCount;               // 0x0364
    u32 m_ConstCommand[EXE_COUNT_MAX][0x125 * 2]; // 0x0368, per executable
    u32 m_ConstCommandCount[EXE_COUNT_MAX];  // 0x12868
};
ASSERT_SIZE(Shader, 0x128E8);
} // namespace CTR
} // namespace gr
} // namespace nn
