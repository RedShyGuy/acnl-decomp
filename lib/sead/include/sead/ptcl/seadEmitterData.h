#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_Texture.h"
#include "sead/seadColor4f.h"
#include "sead/seadMatrix.h"
#include "sead/seadVector2.h"
#include "sead/seadVector3.h"

namespace sead {
namespace ptcl {
// the description of a texture in the resource (member names are ours)
struct TextureRes
{
    u16 width;     // 0x0
    u16 height;    // 0x2
    u16 format;    // 0x4, the low byte is the format of nn::gr
    u8 mipLevel;   // 0x6
    u8 wrap;       // 0x7, low nibble s, high nibble t: 0 repeat, 1 mirror, 2 clamp
    u8 filter;     // 0x8, 0 linear
    u8 _pad[7];
};
ASSERT_SIZE(TextureRes, 0x10);

// the texture of an emitter: the unit 0 settings and the words of its registers (member names are
// ours)
struct TextureHandle
{
    nn::gr::CTR::Texture texture; // 0x00
    u32 param;                     // 0xA8, the filter and wrap register
    u32 size;                      // 0xAC, width << 16 | height
    u32 lod;                       // 0xB0
};
ASSERT_SIZE(TextureHandle, 0xB4);

// the children of the particles of a complex emitter (member names are ours)
struct ChildData
{
    // @fields ChildData
    s32 emitRate;                            // 0x00, children per emission
    s32 emitStartPercent;                    // 0x04, percent of the life of the parent
    s32 life;                                // 0x08
    s32 emitInterval;                        // 0x0C
    f32 velInheritRate;                      // 0x10, of the movement of the parent (childFlag 0x20)
    f32 figureVelocity;                      // 0x14, along the random direction
    Vector3<f32> randomVelocity;             // 0x18
    f32 initPosRandom;                       // 0x24, the radius around the parent
    s32 blendType;                           // 0x28
    u32 billboardType;                       // 0x2C
    s32 depthAlphaType;                      // 0x30
    TextureRes textureRes;                   // 0x34
    u32 textureDataOffset;                   // 0x44
    TextureHandle* texture;                  // 0x48
    Color4f color;                           // 0x4C
    Vector3<f32> combinerColor;              // 0x5C, 0 to 255
    f32 alpha;                               // 0x68, between the two sections
    f32 alphaTarget;                         // 0x6C, at the end of the life
    f32 alphaInit;                           // 0x70
    f32 scaleInheritRate;                    // 0x74, childFlag 8
    Vector2<f32> scale;                      // 0x78
    s32 ptclCalcType;                        // 0x80, the rotation (none, x, y, z, xyz)
    u32 rotInit[3];                          // 0x84, angle indices
    u32 rotInitRandom[3];                    // 0x90
    u32 rotVel[3];                           // 0x9C
    u32 rotVelRandom[3];                     // 0xA8
    Vector2<f32> billboardParam;             // 0xB4
    Vector3<f32> gravity;                    // 0xBC
    s32 alphaSection2;                       // 0xC8, frames
    s32 alphaSection1;                       // 0xCC
    s32 scaleSection;                        // 0xD0
    Vector2<f32> scaleTarget;                // 0xD4, factor at the end of the life
    Vector2<f32> texScale;                   // 0xDC
    s32 combinerType;                        // 0xE4
    f32 airRegist;                           // 0xE8
    // @end
};

// The data of an emitter in the resource (the class name is from the reference symbols, the member
// names are ours).
struct SimpleEmitterData
{
    // @fields SimpleEmitterData
    s32 type;                                // 0x00, the EmitterCalc of PtclSystem (0 simple, 1 complex, 2 simple GPU)
    u32 flag;                                // 0x04, 1: the color of a particle is one of colors by random
    u32 randomSeed;                          // 0x08, 0: from the global random generator
    u32 nameOffset;                          // 0x0C, in the string table
    const char* name;                        // 0x10
    TextureRes textureRes;                   // 0x14
    u32 textureDataOffset;                   // 0x24, in the texture data
    TextureHandle* texture;                  // 0x28
    bool isEmitterBillboard;                 // 0x2C, the particles turn with the emitter (3 rows of the rotation)
    u8 ptclFollowType;                       // 0x2D, 0: the particles keep the matrices of their emission
    bool isRotateEmitter;                    // 0x2E, the billboard block has the rotation of the emitter
    bool isWorldGravity;                     // 0x2F, the gravity is in the space of the emitter
    bool isTexPatAnim;                       // 0x30
    bool isDirectional;                      // 0x31, the particles face along their velocity
    u8 sphereDivTblIndex;                    // 0x32
    bool isStopEmitInFade;                   // 0x33
    s32 volumeType;                          // 0x34, the emit function
    Vector3<f32> volumeScale;                // 0x38
    u32 volumeArcStart;                      // 0x44, angle index
    u32 volumeArcWidth;                      // 0x48
    f32 figureVelocity;                      // 0x4C, along the shape of the volume
    Vector3<f32> emitterVelocityDir;         // 0x50
    f32 emitterVelocity;                     // 0x5C
    f32 initVelocityRandom;                  // 0x60
    Vector3<f32> spreadVec;                  // 0x64
    s32 emitStartFrame;                      // 0x70
    s32 emitEndFrame;                        // 0x74
    s32 emitInterval;                        // 0x78
    s32 emitIntervalRandom;                  // 0x7C
    s32 emitRate;                            // 0x80, 8.8 fixed point
    s32 ptclLife;                            // 0x84, 0x7FFFFFFF: infinite
    s32 ptclLifeRandom;                      // 0x88
    f32 airRegist;                           // 0x8C
    s32 blendType;                           // 0x90
    u32 billboardType;                       // 0x94
    s32 depthAlphaType;                      // 0x98
    Vector3<f32> gravity;                    // 0x9C
    Color4f colors[3];                       // 0xA8, one of them by random (flag 1) or one after the other
    Vector3<f32> combinerColor;              // 0xD8, 0 to 255
    s32 colorSection1;                       // 0xE4, percent of the period
    s32 colorSection2;                       // 0xE8
    s32 colorSection3;                       // 0xEC
    s32 colorNumRepeat;                      // 0xF0
    f32 alphaInit;                           // 0xF4
    f32 alphaDiff1;                          // 0xF8
    f32 alphaDiff2;                          // 0xFC
    s32 alphaSection1;                       // 0x100, percent of the life
    s32 alphaSection2;                       // 0x104
    Vector2<f32> scaleInit;                  // 0x108
    Vector2<f32> scaleDiff1;                 // 0x110
    Vector2<f32> scaleDiff2;                 // 0x118
    s32 scaleSection1;                       // 0x120
    s32 scaleSection2;                       // 0x124
    f32 scaleRandom;                         // 0x128
    s32 ptclCalcType;                        // 0x12C, the block writer: rotation (none, x, y, z, xyz) + 5 * color mode
    s32 preCalcType;                         // 0x130, the follow type (+3 for complex emitters)
    s32 combinerType;                        // 0x134
    u32 rotInit[3];                          // 0x138, angle indices
    u32 rotInitRandom[3];                    // 0x144
    u32 rotVel[3];                           // 0x150
    u32 rotVelRandom[3];                     // 0x15C
    Vector2<f32> billboardParam;             // 0x168
    Matrix34<f32> emitterSRT;                // 0x170
    Matrix34<f32> emitterRT;                 // 0x1A0
    f32 fadeSpeed;                           // 0x1D0
    u16 texPatNum;                           // 0x1D4
    u8 texPatDivX;                           // 0x1D6
    u8 _pad_0x1D7[0x1];
    Vector2<f32> texPatScale;                // 0x1D8
    u8 texPatTbl[16];                        // 0x1E0
    s16 texPatFreq;                          // 0x1F0, frames per pattern, 0: over the life
    s16 texPatTblUse;                        // 0x1F2
    u16 childFlag;                           // 0x1F4, complex: the ChildFlag bits of seadEmitterComplexCalc.cpp (1 the particles have children)
    u16 fieldFlag;                           // 0x1F6, complex: 1 random, 2 magnet, 4 spin, 8 collision, 16 convergence, 32 position add
    u16 fluctuationFlag;                     // 0x1F8, complex: 1 on, 2 alpha, 4 scale
    u16 stripeFlag;                          // 0x1FA, complex: 1 the stripes follow the emitter
    u8 _pad_0x1FC[0x2];
    u16 fieldDataOffset;                     // 0x1FE, complex: from the start of the data
    u16 fluctuationDataOffset;               // 0x200
    u16 stripeDataOffset;                    // 0x202
    u8 _pad_0x204[0x4];
    ChildData child;                         // 0x208, complex
    // @end
};

// the data of the fields of a complex emitter, one after the other from fieldDataOffset in the order of
// the bits of fieldFlag (member names are ours)
struct FieldRandomData
{
    s32 interval;         // frames
    Vector3<f32> power;
};
ASSERT_SIZE(FieldRandomData, 0x10);

struct FieldMagnetData
{
    f32 power;
    Vector3<f32> pos;
    u32 flag;             // 1 x, 2 y, 4 z
};
ASSERT_SIZE(FieldMagnetData, 0x14);

struct FieldSpinData
{
    u32 angle;            // per frame
    s32 axis;             // 0 x, 1 y, 2 z
};
ASSERT_SIZE(FieldSpinData, 0x8);

struct FieldCollisionData
{
    u16 type;             // 0 kills, 1 bounces
    u16 isWorld;
    f32 y;
    f32 bounce;
};
ASSERT_SIZE(FieldCollisionData, 0xC);

struct FieldConvergenceData
{
    Vector3<f32> pos;
    f32 ratio;
};
ASSERT_SIZE(FieldConvergenceData, 0x10);

struct FieldPosAddData
{
    Vector3<f32> pos;
};
ASSERT_SIZE(FieldPosAddData, 0xC);

// the stripes of a complex emitter (at stripeDataOffset; member names are ours)
struct StripeData
{
    // @fields StripeData
    s32 type;                                // 0x00, 0 billboard, 1 across the movement, 2 the y axis of the emitter
    s32 numHistoryMax;                       // 0x04, the size of the ring of history entries
    f32 alphaStart;                          // 0x08
    f32 alphaEnd;                            // 0x0C
    f32 texScrollU;                          // 0x10
    f32 texScrollV;                          // 0x14
    s32 numDraw;                             // 0x18, the history entries drawn at most
    f32 dirInterpolate;                      // 0x1C, how fast the direction follows the movement
    // @end
};

// the fluctuation of a complex emitter (at fluctuationDataOffset; member names are ours)
struct FluctuationData
{
    // @fields FluctuationData
    f32 amplitude;                           // 0x00
    f32 frequency;                           // 0x04
    s32 phaseRandom;                         // 0x08
    // @end
};
} // namespace ptcl
} // namespace sead
