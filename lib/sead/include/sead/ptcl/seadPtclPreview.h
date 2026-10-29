#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"
#include "sead/ptcl/seadTextureData.h"
#include "sead/seadColor4f.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadMatrix.h"
#include "sead/seadVector3.h"

namespace sead {
class Heap;

namespace ptcl {
class PtclModelMgr;
class PtclProject;
class PtclProjectEmitterSet;
struct Handle;
struct PtclEditorSetting;

// RTTI N4sead4ptcl11PtclPreviewE @ 0x008D2038
// vtable 0x00906AC4 (vptr 0x00906ACC), offset_to_top 0, 3 entries
// The preview of the editor: the models, the background and the movement of the emitter sets (the class and
// the function names are from the reference symbols, the member names are ours).
class PtclPreview : public ::sead::hostio::Node
{
public:
    static const s32 NUM_MODEL = 5;

    PtclPreview(Heap* heap, PtclEditorSetting* setting, PtclProject* project); // 0x0055554C
    virtual ~PtclPreview(); // 0x005556EC slot 0x04
    // 0x005556CC slot 0x08 (deleting dtor)

    // the automatic movement from its start
    void resetAutoMove_(bool resetFrame); // 0x005552E0 | mk7dlp:bytes [tier B]
    // the matrix of the preview into the emitter set of handle (at a model or a bone by the settings of the game)
    void applyToEmitterSet(PtclProjectEmitterSet* set, Handle* handle); // 0x00555390 | mk7dlp:bytes-fuzzy [tier B]
    // the models start again
    void start(); // 0x00555518 | mk7dlp:bytes [tier B]

    Heap* mHeap;                             // 0x004
    PtclProject* mProject;                   // 0x008
    PtclModelMgr* mModels[NUM_MODEL];        // 0x00C
    s32 mNumValidModel;                      // 0x020
    PtclModelMgr* mValidModels[NUM_MODEL];   // 0x024
    FixedSafeString<256> mName;              // 0x038
    Color4f mBgColor;                        // 0x144
    TextureData mBgTexture;                  // 0x154
    PtclEditorSetting* mSetting;             // 0x18C
    Matrix34<f32> mViewMtx;                  // 0x190, for the billboards of the game settings
    s32 mAutoMoveFrame;                      // 0x1C0
    u32 mAutoMoveRot;                        // 0x1C4, angle index
    u32 mAutoMoveRotSpeed;                   // 0x1C8
    f32 mAutoMoveRadius;                     // 0x1CC
    Matrix34<f32> mMtx;                      // 0x1D0
    u32 mAutoMoveRotZSpeed;                  // 0x200
    f32 field_0x204;
    u32 field_0x208;
    u32 field_0x20C;
    Vector3<f32> mAutoMoveTranslate;         // 0x210
    u32 field_0x21C;
    s32 mInterval;                           // 0x220, frames (180)
    bool field_0x224;
};
ASSERT_OFFSET(PtclPreview, mModels, 0x0C);
ASSERT_OFFSET(PtclPreview, mName, 0x38);
ASSERT_OFFSET(PtclPreview, mBgTexture, 0x154);
ASSERT_OFFSET(PtclPreview, mSetting, 0x18C);
ASSERT_OFFSET(PtclPreview, mMtx, 0x1D0);
ASSERT_SIZE(PtclPreview, 0x228);
} // namespace ptcl
} // namespace sead
