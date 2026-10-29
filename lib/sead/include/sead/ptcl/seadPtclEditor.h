#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"
#include "sead/ptcl/seadEmitterSet.h"
#include "sead/seadColor4f.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadMatrix.h"
#include "sead/seadSafeStringBase.h"
#include "sead/seadVector3.h"

namespace sead {
class Heap;

namespace ptcl {
class PtclEditor;
class PtclEditorCamera;
class PtclLoadCheck;
class PtclPreview;
class PtclProject;
class PtclProjectEmitterSet;
class PtclResource;
class PtclSystem;

// The ranges of the sliders of the editor and their meta strings for the host (names are ours).
struct PtclEditorSettingRange
{
    PtclEditorSettingRange(); // (inline in PtclEditorSetting::PtclEditorSetting)

    s32 field_0x00;                             // 0x000
    f32 mRange0;                              // 0x004
    f32 mRange1;                              // 0x008
    f32 mRange2;                            // 0x00C
    f32 mRange3;                            // 0x010
    s32 mFrameRange;                            // 0x014
    FixedSafeString<64> mMetaRange0;               // 0x018, -mRange0 to mRange0
    FixedSafeString<64> mMetaRange0Max;            // 0x064, 0 to mRange0
    FixedSafeString<64> mMetaRange2;             // 0x0B0
    FixedSafeString<64> mMetaRange2Max;          // 0x0FC
    FixedSafeString<64> mMetaRange2Tenth;        // 0x148
    FixedSafeString<96> mMetaRange2MaxMenu;      // 0x194
    FixedSafeString<96> mMetaRange1;               // 0x200
    FixedSafeString<96> mMetaRange1Max;            // 0x26C
    FixedSafeString<96> mMetaRange3;             // 0x2D8
    FixedSafeString<96> mMetaRange3Max;          // 0x344
    FixedSafeString<64> mMetaFrame;             // 0x3B0
    FixedSafeString<64> mMetaFrameFrom1;        // 0x3FC
    FixedSafeString<64> mMetaRange0Off;            // 0x448, the same disabled
    FixedSafeString<64> mMetaRange0MaxOff;         // 0x494
    FixedSafeString<64> mMetaRange2MaxOff;       // 0x4E0
    FixedSafeString<96> mMetaRange1Off;            // 0x52C
    FixedSafeString<96> mMetaRange1MaxOff;         // 0x598
    FixedSafeString<64> mMetaRange3Off;          // 0x604
    FixedSafeString<96> mMetaRange3MaxOff;       // 0x650
    FixedSafeString<64> mMetaFrameOff;          // 0x6BC
    FixedSafeString<64> mMetaFrameFrom1Off;     // 0x708
    FixedSafeString<64> mMetaLongFrame;         // 0x754, to 10 times mFrameRange
    FixedSafeString<64> mMetaLongFrameFrom1;    // 0x7A0
    FixedSafeString<64> mMetaLongFrameOff;      // 0x7EC
    FixedSafeString<64> mMetaLongFrameFrom1Off; // 0x838
};
ASSERT_OFFSET(PtclEditorSettingRange, mMetaRange0, 0x18);
ASSERT_OFFSET(PtclEditorSettingRange, mMetaRange2MaxMenu, 0x194);
ASSERT_OFFSET(PtclEditorSettingRange, mMetaFrame, 0x3B0);
ASSERT_OFFSET(PtclEditorSettingRange, mMetaLongFrameFrom1Off, 0x838);
ASSERT_SIZE(PtclEditorSettingRange, 0x884);

// The light and the colors of the preview (names are ours).
struct PtclEditorSettingLight
{
    PtclEditorSettingLight(); // (inline in PtclEditorSetting::PtclEditorSetting)

    Vector3<f32> mDir;        // 0x00
    Color4f mColor0;          // 0x0C
    Color4f mColor1;          // 0x1C
    Color4f mColor2;          // 0x2C
    Color4f mColor3;          // 0x3C
    Vector3<f32> mUp;         // 0x4C
    Color4f mColor4;          // 0x58
    Color4f mColor5;          // 0x68
    f32 mValue0;              // 0x78
    Color4f mColor6;         // 0x7C
    f32 mValue1;              // 0x8C
    f32 mValue2;              // 0x90
    f32 mValue3;              // 0x94
    s32 mType;                // 0x98
    u8 _unk_0x9C[0x30];
};
ASSERT_OFFSET(PtclEditorSettingLight, mColor4, 0x58);
ASSERT_OFFSET(PtclEditorSettingLight, mType, 0x98);
ASSERT_SIZE(PtclEditorSettingLight, 0xCC);

// The settings of the editor (names are ours).
struct PtclEditorSetting
{
    explicit PtclEditorSetting(PtclEditor* editor); // 0x0055AA08

    PtclEditor* mEditor;                  // 0x000
    Matrix34<f32> mMtx;                   // 0x004
    PtclEditorSettingRange mRange;        // 0x034
    PtclEditorSettingLight mLight;        // 0x8B8
    FixedSafeString<64> mIndexNames[16];  // 0x984, "[0]" to "[15]"
    FixedSafeString<64> mValueNames[2];   // 0xE44
    PtclEditorCamera* mCamera;            // 0xEDC
    f32 mFrameRate;                       // 0xEE0
};
ASSERT_OFFSET(PtclEditorSetting, mLight, 0x8B8);
ASSERT_OFFSET(PtclEditorSetting, mIndexNames, 0x984);
ASSERT_OFFSET(PtclEditorSetting, mCamera, 0xEDC);
ASSERT_SIZE(PtclEditorSetting, 0xEE4);

// RTTI N4sead4ptcl10PtclEditorE @ 0x008D2004
// vtable 0x00906A94 (vptr 0x00906A9C), offset_to_top 0, 4 entries
// The particle editor on the host: the project it sent, the preview and the emitter sets created for it. (The
// RTTI also lists hostio::PaletteEventListener at offset 4, which the code does not show: the constructor
// stores the heap there; it is left out.) The class and the function names are from the reference symbols,
// the member names are ours.
class PtclEditor : public ::sead::hostio::Node
{
public:
    static const s32 NUM_EMITTER_SET = 64;
    static const s32 NUM_HANDLE = 128;

    PtclEditor(Heap* heap, PtclSystem* system, s32 resId); // 0x0054EBB0 (name is ours)
    virtual ~PtclEditor(); // 0x0054EF04 slot 0x04
    // 0x0054EEF4 slot 0x08 (deleting dtor)
    // an emitter set of the project was deleted (name is ours)
    virtual void onRemoveEmitterSet(PtclProjectEmitterSet* set); // 0x0054EB44 slot 0x0C

    // kills the emitter sets of the preview (or lets them fade out)
    DECOMP_NOINLINE void stopPreview(bool kill); // 0x0054E778 | mk7dlp:bytes [tier B]
    // creates the emitter sets of the project again (name is ours)
    void startPreview(bool kill); // 0x0054E9B8
    // what the project has bound to resource
    void clearResource(PtclResource* resource); // 0x0054E80C | mk7dlp:bytes [tier B]
    // binds the emitter sets of the project to the emitter sets of resource with their names (name is ours)
    void bindResource(PtclResource* resource); // 0x0054E844

    Heap* mHeap;                                         // 0x00004
    PtclProject* mProject;                               // 0x00008
    s32 mResId;                                          // 0x0000C, of the resource of the editor
    PtclSystem* mSystem;                                 // 0x00010
    u32 field_0x14;                                      // 0x00014
    u8* field_0x18;                                      // 0x00018
    PtclPreview* mPreview;                               // 0x0001C
    PtclEditorSetting mSetting;                          // 0x00020
    PtclLoadCheck* mLoadCheck;                           // 0x00F04
    PtclEditorCamera* mCamera;                           // 0x00F08
    s32 field_0xF0C;                                     // 0x00F0C, -1
    s32 field_0xF10;                                     // 0x00F10, -1
    Handle mHandles[NUM_EMITTER_SET][NUM_HANDLE];        // 0x00F14
    u8 mHandleIndex[NUM_EMITTER_SET];                    // 0x10F14, the last handle of each set
    s32 mPreviewFrame;                                   // 0x10F54
    s32 mPreviewInterval;                                // 0x10F58, frames (180)
    bool mIsPause;                                       // 0x10F5C
    bool mIsAutoStart;                                   // 0x10F5D
    bool field_0x10F5E;                                  // 0x10F5E
    s32 field_0x10F60;                                   // 0x10F60
    s32 field_0x10F64;                                   // 0x10F64
    s32 field_0x10F68;                                   // 0x10F68
    bool field_0x10F6C;                                  // 0x10F6C
    bool field_0x10F6D;                                  // 0x10F6D
    void* mBuffer0;                                      // 0x10F70, 512 KiB
    void* mBuffer1;                                      // 0x10F74, 512 KiB
    s32 field_0x10F78;                                      // 0x10F78
    s32 field_0x10F7C;                                      // 0x10F7C
    void* mBuffer2;                                      // 0x10F80, 160 KiB
    void* mBuffer3;                                      // 0x10F84, 160 KiB
    void* mIndexBuffer;                                  // 0x10F88, the indices of 4096 quads
    s32 field_0x10F8C;                                   // 0x10F8C
    SafeStringBase<char> mEmptyName;                              // 0x10F90
    u8 field_0x10F98[80];                                // 0x10F98
};
ASSERT_OFFSET(PtclEditor, mSetting, 0x20);
ASSERT_OFFSET(PtclEditor, mLoadCheck, 0xF04);
ASSERT_OFFSET(PtclEditor, mHandles, 0xF14);
ASSERT_OFFSET(PtclEditor, mHandleIndex, 0x10F14);
ASSERT_OFFSET(PtclEditor, mBuffer0, 0x10F70);
ASSERT_OFFSET(PtclEditor, mEmptyName, 0x10F90);
ASSERT_SIZE(PtclEditor, 0x10FE8);
} // namespace ptcl
} // namespace sead
