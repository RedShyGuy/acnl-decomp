#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"

// RTTI 12SoundObjBase @ 0x008CB818
// vtable 0x008EE838 (vptr 0x008EE840), offset_to_top 0, 7 entries
// A sound source in the world: from its position relative to the listener it gets a distance volume, a pan and a surround
// pan by the distance curves of its type (the class name is from the RTTI, everything else is ours).
class SoundObjBase
{
public:
    // the curves of a type (m_Type)
    struct TypeParam
    {
        f32 nearDistance;      // 0x00 full volume up to here
        f32 farDistance;       // 0x04 silent from here on
        nn::math::VEC2 inner;  // 0x08 x: no pan up to here, y: the surround pan starts here
        nn::math::VEC2 outer;  // 0x10 x: full pan from here on, y: the surround pan ends here
        f32 positionOffsetZ;   // 0x18 added to the relative position
    };
    ASSERT_SIZE(TypeParam, 0x1C);

    enum
    {
        TYPE_NUM = 14,
    };

    SoundObjBase(); // 0x0020B4D4
    virtual ~SoundObjBase(); // 0x0020B570 slot 0x00
    // 0x0020B56C slot 0x04 (deleting dtor)
    // the first position of the object (the stage decides the type)
    virtual void Start(const nn::math::VEC3* world, const nn::math::VEC3* relative); // 0x0020AA88 slot 0x08
    virtual void Stop(); // 0x0020B498 slot 0x0C
    virtual void SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative); // 0x0020B3E8 slot 0x10
    // true if Calc ran because something changed
    virtual bool UpdateChanged(); // 0x0020B264 slot 0x14
    // the volume, pan and surround pan from the position relative to the listener
    virtual void Calc(); // 0x0020AC30 slot 0x18

    void SetType(u32 type); // 0x0020AC08
    void SetTypeEntry11(); // 0x0020B270
    void SetIsUseHeight(bool isUseHeight); // 0x0020B280
    void SetIsNoDistance(bool isNoDistance); // 0x0020B2A4
    // (never on the title screen)
    void SetIsIgnoreMaster(bool isIgnoreMaster); // 0x0020B2C8
    // the height volume and the type after a stage change
    void OnEnterStage(); // 0x0020B308
    // the train fades when the listener is high up
    void UpdateTrainFade(u32 soundId); // 0x0020B364
    void SetIsFaded(bool isFaded); // 0x0020B3C8
    // the distance curves of the BGM follow this object
    void BindBgm(); // 0x0020B444

    static TypeParam s_TypeParams[TYPE_NUM];

    s32 m_Priority;        // 0x04
    f32 m_PosX;            // 0x08 relative to the listener
    f32 m_PosZ;            // 0x0C
    f32 m_WorldHeight;     // 0x10
    f32 m_VolumeScale;     // 0x14
    f32 m_DistanceVolume;  // 0x18
    f32 m_Volume;          // 0x1C
    f32 m_Pan;             // 0x20
    f32 m_SurroundPan;     // 0x24
    s32 m_PlayerPriority;  // 0x28 how much the priority of the sounds drops by the distance
    f32 m_Distance;        // 0x2C
    f32 m_BackScale;       // 0x30 of the distance behind the listener
    bool m_IsActive;       // 0x34
    s8 m_IsIgnoreMaster;   // 0x35 not scaled by the master fader of the volume manager
    s8 m_IsNoDistance;     // 0x36 the distance volume is always 1
    s8 m_IsUseHeight;      // 0x37 scaled by SoundMgr::GetHeightVolume
    bool m_IsAlwaysInRange; // 0x38
    s8 m_IsFaded;          // 0x39 follows the fader of SoundMgr
    s32 m_Type;            // 0x3C
    s32 m_Counter;         // 0x40 raised by each change
};
ASSERT_OFFSET(SoundObjBase, m_IsActive, 0x34);
ASSERT_OFFSET(SoundObjBase, m_Type, 0x3C);
ASSERT_SIZE(SoundObjBase, 0x44);
