#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResShader.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "nw/gfx/res/gfx_ResTextureCoordinatorData.h"
#include "nw/ut/internal/ut_ResArray.h"
#include "nw/ut/ut_FloatColor.h"

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

// The colors of a material: as floats and as 8-bit colors (ut::Color8 words; names are ours)
struct ResMaterialColorData
{
    ut::FloatColor emission;      // 0x00
    ut::FloatColor ambient;       // 0x10
    ut::FloatColor diffuse;       // 0x20
    ut::FloatColor specular0;     // 0x30
    ut::FloatColor specular1;     // 0x40
    ut::FloatColor constant[6];   // 0x50
    u32 emissionU8;               // 0xB0
    u32 ambientU8;                // 0xB4
    u32 diffuseU8;                // 0xB8
    u32 specular0U8;              // 0xBC
    u32 specular1U8;              // 0xC0
    u32 constantU8[6];            // 0xC4
    u32 m_Unknown0xDC;            // 0xDC
};
ASSERT_SIZE(ResMaterialColorData, 0xE0);

// the wrapper of the colors of a material (the class name is from the reference symbols)
class ResMaterialColor
{
public:
    // (inline)
    explicit ResMaterialColor(const void* pData = nullptr) : m_pData(static_cast<const ResMaterialColorData*>(pData)) {}
    const ResMaterialColorData* ptr() const { return m_pData; }

    const ResMaterialColorData* m_pData; // 0x0
};
ASSERT_SIZE(ResMaterialColor, 0x4);

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
    // rasterizationFlags (names are ours)
    enum RasterizationFlag
    {
        RASTERIZATION_POLYGON_OFFSET = 1 << 0,
    };
    // depthFlags (names are ours)
    enum DepthFlag
    {
        DEPTH_TEST = 1 << 0,
        DEPTH_WRITE = 1 << 1,
    };
    // in flags of the resource of Material::BUFFER_SHADING (names are ours; FLAG_FRAGMENT_LIGHTING too)
    static const u32 FLAG_HEMISPHERE_LIGHTING = 1 << 2;
    static const u32 FLAG_HEMISPHERE_OCCLUSION = 1 << 3;
    static const u32 FLAG_FOG = 1 << 4;

    u32 typeInfo;                        // 0x000
    u32 signature;                       // 0x004 "MTOB"
    u32 revision;                        // 0x008
    ut::Offset name;                     // 0x00C
    s32 userDataCount;                   // 0x010
    ut::Offset userDataDic;              // 0x014
    u32 flags;                           // 0x018
    u32 textureCoordinateConfig;         // 0x01C which coordinators the texture units use (low byte)
    u32 m_Unknown0x20;                   // 0x020
    ResMaterialColorData materialColor;  // 0x024 (animated as a whole)
    u32 rasterizationFlags;              // 0x104 RasterizationFlag
    u32 m_Unknown0x108;                  // 0x108
    f32 polygonOffsetUnits;              // 0x10C
    u32 cullingCommand[2];               // 0x110 (FACECULLING_CONFIG)
    u32 depthFlags;                      // 0x118 DepthFlag
    u32 depthCommand[4];                 // 0x11C
    u32 fragmentOperationMode;           // 0x12C low byte: the blend mode (3: logic op); the blend operation starts here
    u32 m_Unknown0x130[4];               // 0x130
    u32 colorOperation;                  // 0x140 the blend commands start here (COLOR_OPERATION, six words)
    u32 colorOperationHeader;            // 0x144
    u32 blendFunction;                   // 0x148 (3dbrew "GPU/Internal Registers": BLEND_FUNC)
    u32 logicOperation;                  // 0x14C (LOGIC_OP)
    u32 m_Unknown0x150[2];               // 0x150
    u32 stencilCommand[4];               // 0x158 (word 0 bit 0: the stencil test is used)
    s32 textureCoordinatorCount;         // 0x168 the coordinators that are used
    ResTextureCoordinatorData textureCoordinators[3]; // 0x16C
    ut::Offset textureMappers[3];        // 0x274 ResPixelBasedTextureMapperData
    ut::Offset proceduralTextureMapper;  // 0x280
    ut::Offset shader;                   // 0x284 ResShaderData
    ut::Offset fragmentShader;           // 0x288 ResFragmentShaderData
    s32 shaderProgramDescriptionIndex;   // 0x28C
    ResArrayInfo userUniforms;           // 0x290 ResUserUniformData
    s32 lightSetIndex;                   // 0x298 the light set of the scene environment
    s32 fogIndex;                        // 0x29C the fog of the scene environment
    u32 shadingHash;                     // 0x2A0
    u32 userUniformsHash;                // 0x2A4
    u32 textureCoordinatorsHash;         // 0x2A8
    u32 textureSamplersHash;             // 0x2AC
    u32 textureMappersHash;              // 0x2B0 the samplers hash with the textures of the mappers
    u32 materialColorHash;               // 0x2B4
    u32 rasterizationHash;               // 0x2B8
    u32 fragmentLightingHash;            // 0x2BC
    u32 fragmentLightingTableHash;       // 0x2C0 the seed with the lookup tables of the samplers
    u32 fragmentLightingTableHashSeed;   // 0x2C4
    u32 textureCombinersHash;            // 0x2C8
    u32 combinerBufferHash;              // 0x2CC
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
