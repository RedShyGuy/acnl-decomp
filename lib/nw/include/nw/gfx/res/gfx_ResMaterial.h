#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResShader.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "nw/gfx/res/gfx_ResTextureCoordinatorData.h"
#include "nw/ut/internal/ut_ResArray.h"

namespace nw {
namespace gfx {
namespace res {
// a uniform of the program that the material sets (names are ours)
struct ResUserUniformData
{
    // the kinds of the values (names are ours; a bool or an int is one value, a vector one per element)
    enum Type
    {
        TYPE_BOOL = 0,
        TYPE_INT = 1,
        TYPE_VEC2 = 2,
        TYPE_VEC3 = 3,
        TYPE_VEC4 = 4,
    };

    ut::Offset name;  // 0x0
    s32 symbolIndex;  // 0x4 the symbol of the program description of the same name (set by Setup)
    u32 type;         // 0x8 Type (low byte)
    f32 values[1];    // 0xC as many as the type has

    // (inline) the values of a type
    static int GetValueCount(u8 type)
    {
        switch (type) {
        case TYPE_BOOL:
        case TYPE_INT:
            return 1;
        case TYPE_VEC2:
            return 2;
        case TYPE_VEC3:
            return 3;
        case TYPE_VEC4:
            return 4;
        default:
            return 0;
        }
    }
};
// the size of a user uniform without its values
const u32 USER_UNIFORM_HEADER_SIZE = 0xC;

// The data of a material resource (only the fields known so far; names are ours). The hashes say
// which state the material activator sent last; an animation of a part sets its hash to 0, so the
// part is sent again.
struct ResMaterialData
{
    // the setup state and settings in flags
    enum Flag
    {
        FLAG_FRAGMENT_LIGHTING = 0x1,
        FLAG_SETUP_DONE = 0x40,
        FLAG_SHADER_SETUP_DONE = 0x80,
        FLAG_TEXTURES_SETUP_DONE = 0x100,
        FLAG_FRAGMENT_SHADER_SETUP_DONE = 0x200,
    };
    // in fragmentOperationMode: the result does not depend on the color buffer (set by Setup)
    static const u32 FRAGMENT_OPERATION_NO_DESTINATION = 0x80000000;

    u32 typeInfo;                        // 0x000
    u32 signature;                       // 0x004 "MTOB"
    u32 revision;                        // 0x008
    ut::Offset name;                     // 0x00C
    s32 userDataCount;                   // 0x010
    ut::Offset userDataDic;              // 0x014
    u32 flags;                           // 0x018
    u32 textureCoordinateConfig;         // 0x01C which coordinators the texture units use (low byte)
    u32 m_Unknown0x20;                   // 0x020
    u8 materialColor[0xE0];              // 0x024 the colors (animated as a whole)
    u8 m_Unknown0x104[0x28];             // 0x104
    u32 fragmentOperationMode;           // 0x12C low byte: the blend mode (3: logic op); the blend operation starts here
    u8 m_Unknown0x130[0x18];             // 0x130
    u32 blendFunction;                   // 0x148 (3dbrew "GPU/Internal Registers": BLEND_FUNC)
    u32 logicOperation;                  // 0x14C (LOGIC_OP)
    u8 m_Unknown0x150[0x1C];             // 0x150
    ResTextureCoordinatorData textureCoordinators[3]; // 0x16C
    ut::Offset textureMappers[3];        // 0x274 ResPixelBasedTextureMapperData
    ut::Offset proceduralTextureMapper;  // 0x280
    ut::Offset shader;                   // 0x284 ResShaderData
    ut::Offset fragmentShader;           // 0x288 ResFragmentShaderData
    s32 shaderProgramDescriptionIndex;   // 0x28C
    ResArrayInfo userUniforms;           // 0x290 ResUserUniformData
    u32 m_Unknown0x298[2];               // 0x298
    u32 m_Unknown0x2A0;                  // 0x2A0 (a hash: cleared in a copy)
    u32 m_Unknown0x2A4;                  // 0x2A4 (a hash: cleared in a copy)
    u32 textureCoordinatorsHash;         // 0x2A8
    u32 textureSamplersHash;             // 0x2AC
    u32 textureMappersHash;              // 0x2B0 the samplers hash with the textures of the mappers
    u32 materialColorHash;               // 0x2B4
    u32 m_Unknown0x2B8;                  // 0x2B8
    u32 fragmentLightingHash;            // 0x2BC
    u32 fragmentLightingTableHash;       // 0x2C0 the seed with the lookup tables of the samplers
    u32 fragmentLightingTableHashSeed;   // 0x2C4
    u32 m_Unknown0x2C8;                  // 0x2C8 (a hash: cleared in a copy)
    u32 m_Unknown0x2CC;                  // 0x2CC (a hash: cleared in a copy)
    u32 fragmentOperationHash;           // 0x2D0
    u32 m_Unknown0x2D4;                  // 0x2D4
};
ASSERT_SIZE(ResMaterialData, 0x2D8);
ASSERT_OFFSET(ResMaterialData, textureCoordinators, 0x16C);
ASSERT_OFFSET(ResMaterialData, userUniforms, 0x290);
ASSERT_OFFSET(ResMaterialData, fragmentOperationHash, 0x2D0);

// the binary resource of a material (the class name and the functions are from the reference symbols)
class ResMaterial
{
public:
    // the result bit of a cube mapping whose texture is no cube (name is ours)
    static const u32 RESULT_NOT_CUBE_TEXTURE = 0x20;

    // (inline)
    explicit ResMaterial(const void* pData = nullptr) : m_pData(static_cast<const ResMaterialData*>(pData)) {}
    const ResMaterialData* ptr() const { return m_pData; }
    ResMaterialData* ptr() { return const_cast<ResMaterialData*>(m_pData); }
    bool IsValid() const { return m_pData != nullptr; }

    void Cleanup(); // 0x0013B800 | nintendogs:bytes [tier A]
    u32 SetupShader(os::IAllocator* pAllocator, ResShader shader, ResGraphicsFile graphicsFile); // 0x004A5CA0 | nintendogs:bytes [tier A]
    u32 SetupTextures(os::IAllocator* pAllocator, ResMaterial material, ResGraphicsFile graphicsFile); // 0x004A5DF4 | nintendogs:bytes-fuzzy [tier A]
    // the textures referenced by the name set to the mappers (without their setup; name is ours)
    void ForceSetupTexture(const char* name, ResTexture texture); // 0x004A618C (name is ours)
    // the symbol index of each user uniform
    void CacheUserUniformIndex(ut::internal::ResArray<ResShaderSymbol, ut::internal::ResArrayClassTraits> symbols); // 0x004A62B0 | nintendogs:bytes [tier A]
    void CalcFragmentLightingTableHash(); // 0x004A6380 | nintendogs:bytes [tier A]
    u32 Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile); // 0x004A662C | nintendogs:bytes [tier A]

    const ResMaterialData* m_pData; // 0x0
};
ASSERT_SIZE(ResMaterial, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
