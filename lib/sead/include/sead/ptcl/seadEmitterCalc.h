#pragma once

#include "decomp.h"
#include "sead/ptcl/seadPtclSystem.h"

namespace sead {
class Heap;

namespace ptcl {
class EmitterInstance;
class PtclInstance;
class PtclRenderer;
class PtclSystem;
struct SimpleEmitterData;
struct TextureHandle;

// RTTI N4sead4ptcl11EmitterCalcE @ 0x008D2030
// vtable 0x00906AB8 (vptr 0x00906AC0), offset_to_top 0, 1 entries
// The calculation of the emitters of a type: the emit functions of the volumes, the fields and the
// blocks of the vertex shader per particle (rotation and color). The functions named in the reference
// symbols are kept; the other names are ours.
class EmitterCalc
{
public:
    // the state that the draw settings of the emitters last sent (names are ours)
    struct CurrentState
    {
        u8 combinerType;
        u8 blendType;
        u8 depthAlphaType;
    };

    typedef void (*EmitFunction)(EmitterInstance* emitter);
    typedef s32 (*PreCalcFunction)(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl);
    typedef u32* (*PtclCalcFunction)(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl);

    explicit EmitterCalc(PtclSystem* system)
    {
        s_pSystem = system;
        s_pRenderer = system->mRenderer;
    }
    // calculates emitter (and its particles) into cmd; the end of the commands, the number of particles
    // in numPtcl (name is ours)
    virtual u32* calcCTR(u32* cmd, EmitterInstance* emitter, s32* numPtcl) = 0;

    // the emit functions by SimpleEmitterData::volumeType
    static void _emitPoint(EmitterInstance* emitter); // 0x005515D4 | mk7dlp:bytes [tier B]
    static void _emitCircle(EmitterInstance* emitter); // 0x005516C0
    static void _emitCircleSameDivide(EmitterInstance* emitter); // 0x00553104
    static void _emitFillCircle(EmitterInstance* emitter); // 0x0055257C
    static void _emitSphere(EmitterInstance* emitter); // 0x00551D94
    static void _emitSphereSameDivide(EmitterInstance* emitter); // 0x00553204 | mk7dlp:bytes [tier B]
    static void _emitFillSphere(EmitterInstance* emitter); // 0x005526D4
    static void _emitCylinder(EmitterInstance* emitter); // 0x005520AC
    static void _emitFillCylinder(EmitterInstance* emitter); // 0x00552AE8
    static void _emitBox(EmitterInstance* emitter); // 0x00554FBC
    static void _emitFillBox(EmitterInstance* emitter); // 0x00551EF8
    static void _emitLine(EmitterInstance* emitter); // 0x005551DC
    static void _emitLineSameDivide(EmitterInstance* emitter); // 0x00552EA4 | mk7dlp:bytes [tier B]
    static void _emitRectangle(EmitterInstance* emitter); // 0x00552208
    // the common start of an emitted particle (random life, animations, matrices; links it in)
    static void initPtcl_(EmitterInstance* emitter, PtclInstance* ptcl); // 0x005517E4

    // the fields of complex emitters: each takes its data and gives the data of the next field
    static const void* _ptclField_Random(EmitterInstance* emitter, PtclInstance* ptcl, const void* data); // 0x00552D34 | mk7dlp:bytes [tier B]
    static const void* _ptclField_Magnet(EmitterInstance* emitter, PtclInstance* ptcl, const void* data); // 0x00552C78 | mk7dlp
    static const void* _ptclField_Spin(EmitterInstance* emitter, PtclInstance* ptcl, const void* data); // 0x0055287C | mk7dlp
    static const void* _ptclField_Collision(EmitterInstance* emitter, PtclInstance* ptcl, const void* data); // 0x00552F98 | mk7dlp
    static const void* _ptclField_Convergence(EmitterInstance* emitter, PtclInstance* ptcl, const void* data); // 0x005532F8 | mk7dlp
    static const void* _ptclField_PosAdd(EmitterInstance* emitter, PtclInstance* ptcl, const void* data); // 0x00552CFC | mk7dlp:bytes [tier B]
    // the alpha and scale of the fluctuation (a cosine table)
    static void calcFluctuation(EmitterInstance* emitter, PtclInstance* ptcl); // 0x00552940 | mk7dlp
    static void setupFluctuation_(Heap* heap); // 0x00552DB8 | mk7dlp:bytes [tier B]

    // the movement of a particle in a frame by how it follows the emitter; the rows of the vertex
    // shader block it wrote (0, 1 for the direction, 3 for the rotation of the emitter)
    static s32 _ptclPreCalc_FollowAll_____Simple(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x005539FC | mk7dlp:bytes [tier B]
    static s32 _ptclPreCalc_FollowNone____Simple(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00553C08 | mk7dlp:bytes [tier B]
    static s32 _ptclPreCalc_FollowPosOnly_Simple(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00553E98 | mk7dlp:bytes [tier B]
    static s32 _ptclPreCalc_FollowAll_____Complex(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00554160 | mk7dlp:bytes [tier B]
    static s32 _ptclPreCalc_FollowNone____Complex(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00554470 | mk7dlp:bytes [tier B]
    static s32 _ptclPreCalc_FollowPosOnly_Complex(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x005547EC | mk7dlp:bytes [tier B]

    // the block of the vertex shader per particle: three rows of (color component, rotation row with
    // the columns z, y, x); the colors of the particle (children), of the emitter (PassColor1) or none
    static u32* _childCalc_RotNone(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00552E40 | mk7dlp:bytes [tier B]
    static u32* _childCalc_RotX(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x005523A0
    static u32* _childCalc_RotY(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00552440
    static u32* _childCalc_RotZ(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x005524DC
    static u32* _childCalc_RotXYZ(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x005529C0
    static u32* _ptclCalc_RotNone_PassColor1(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00553330 | mk7dlp:bytes [tier B]
    static u32* _ptclCalc_RotX___PassColor1(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00553510
    static u32* _ptclCalc_RotY___PassColor1(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x005535B8
    static u32* _ptclCalc_RotZ___PassColor1(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00553660
    static u32* _ptclCalc_RotXYZ__PassColor1(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00553398 | mk7dlp:bytes [tier B]
    static u32* _ptclCalc_RotNone_ColorNoWork(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00553708 | mk7dlp:bytes [tier B]
    static u32* _ptclCalc_RotX___ColorNoWork(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00553888
    static u32* _ptclCalc_RotY___ColorNoWork(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00553904
    static u32* _ptclCalc_RotZ___ColorNoWork(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00553980
    static u32* _ptclCalc_RotXYZ__ColorNoWork(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00553748 | mk7dlp:bytes [tier B]
    static u32* _ptclCalc_RotNone_Interpolate3Colors(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00554BA0 | mk7dlp:bytes [tier B]
    static u32* _ptclCalc_RotX___Interpolate3Colors(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00554DA0
    static u32* _ptclCalc_RotY___Interpolate3Colors(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00554E54
    static u32* _ptclCalc_RotZ___Interpolate3Colors(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00554F08
    static u32* _ptclCalc_RotXYZ__Interpolate3Colors(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl); // 0x00554C18 | mk7dlp:bytes [tier B]

    // the matrix of the particles of emitter (the emitter now or the one of the emission)
    static const Matrix34<f32>* getPtclSRT(EmitterInstance* emitter, PtclInstance* ptcl); // 0x005552C8

    static EmitFunction s_EmitFunctions[14];
    // the block writers by SimpleEmitterData::ptclCalcType and the movements by preCalcType
    static PtclCalcFunction s_PtclCalcFunctions[15];
    static PreCalcFunction s_PreCalcFunctions[6];
    // the block writers of the children by ChildData::ptclCalcType
    static PtclCalcFunction s_ChildCalcFunctions[5];
    // the first word of the block of a quad with extra rows (14 + 9 * index) and of one without them (by
    // SimpleEmitterData::billboardType); built by the static initializer 0x0079D24C
    static f32 s_QuadBlockIndex[8];
    static f32 s_BillboardBlockIndex[4];
    // the fade and the emissions of a frame of emitter; false when the emitter was killed (complex emitters
    // live on while they have children; inline in both calcCTR, the name is ours)
    static bool calcEmission_(EmitterInstance* emitter, bool isComplex);

    static PtclSystem* s_pSystem;
    static PtclRenderer* s_pRenderer;
    // the texture and the state last sent by the draw settings
    static TextureHandle* s_CurrentTexture;
    static CurrentState s_CurrentState;
    // 128 values of (cos + 1) / 2 over a turn
    static f32* s_pFluctuationTbl;
    // the directions of _emitSphereSameDivide by SimpleEmitterData::sphereDivTblIndex
    static const Vector3<f32>* s_SphereDivTbl[8];
};
} // namespace ptcl
} // namespace sead

#include "sead/ptcl/seadEmitterCalc_inl.h"

