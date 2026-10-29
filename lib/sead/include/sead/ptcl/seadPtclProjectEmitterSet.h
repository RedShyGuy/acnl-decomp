#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"
#include "sead/ptcl/seadPtclResource.h"
#include "sead/seadFixedSafeString.h"

namespace sead {
namespace ptcl {
class PtclGameProgrammer;
class PtclResource;
class PtclSystem;

// an emitter of an emitter set of the editor (only the visibility is known; name is ours)
struct PtclProjectEmitter
{
    u8 _unk_0x0[0xD38];
    bool mIsVisible; // 0xD38
};

// An emitter set the editor sent: its emitters, the resources it is bound to and the settings of the game for
// it (the class and the function names are from the reference symbols, the member names are ours).
class PtclProjectEmitterSet : public ::sead::hostio::Node
{
public:
    static const s32 NUM_EMITTER_MAX = 32;

    virtual ~PtclProjectEmitterSet() {}

    // binds the emitter set emitterSetId of resource to the emitters of the set (bind index)
    void bindTarget(s32 index, PtclResource* resource, s32 emitterSetId); // 0x0055C1C8 | mk7dlp:bytes [tier B]
    // what is bound to resource
    void unbindResource(PtclResource* resource); // 0x0055C244 | mk7dlp:bytes-fuzzy [tier B]
    void setOrder(s32 order); // 0x0055C30C | mk7dlp:bytes [tier B]

    // the icon of the node by the state of the binding (inline; name is ours)
    void updateMetaName_()
    {
        const char* icon;
        if (mBind[1].setData == nullptr) {
            icon = "ptcl_set.png";
        } else if (static_cast<s32>(mBind[1].setData->field_0x04) != mNumEmitter) {
            icon = "ptcl_set_bind_error.png";
        } else {
            icon = "ptcl_set_bind.png";
        }
        mMetaName.format("DispName =%s,Icon=%s,Order=%d,Class=PTCLEmitterSet", mName.getCString(), icon, mOrder);
    }

    u8 _unk_0x004[0x110];
    FixedSafeString<64> mName;                     // 0x114
    FixedSafeString<128> mMetaName;                // 0x160
    s32 mOrder;                                    // 0x1EC
    PtclProjectEmitter* mEmitters[NUM_EMITTER_MAX]; // 0x1F0
    ResourceBind mBind[2];                         // 0x270, 0: the data of the editor, 1: the game's
    PtclSystem* mSystem;                           // 0x2A8
    EmitterTblData mEmitterTbl[NUM_EMITTER_MAX];   // 0x2AC
    u8 _unk_0x3AC[0xC];
    bool mIsVisible;                               // 0x3B8
    PtclGameProgrammer* mGameProgrammer;           // 0x3BC
    u32 mUserData;                                 // 0x3C0
    s32 mNumEmitter;                               // 0x3C4
};
ASSERT_OFFSET(PtclProjectEmitterSet, mName, 0x114);
ASSERT_OFFSET(PtclProjectEmitterSet, mOrder, 0x1EC);
ASSERT_OFFSET(PtclProjectEmitterSet, mBind, 0x270);
ASSERT_OFFSET(PtclProjectEmitterSet, mEmitterTbl, 0x2AC);
ASSERT_OFFSET(PtclProjectEmitterSet, mNumEmitter, 0x3C4);
} // namespace ptcl
} // namespace sead
