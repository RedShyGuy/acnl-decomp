#pragma once

#include "decomp.h"
#include "nw/anim/res/anim_ResMemberAnim.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/ut/ut_Offset.h"
#include "nw/ut/ut_ResDicPatricia.h"

namespace nw {
namespace anim {
namespace res {
// An animation: the member animations by the path of the animated member (names are ours).
struct ResAnimData
{
    // how the frame goes on at the end (name is ours)
    enum LoopMode : u8
    {
        LOOP_MODE_ONETIME = 0,
        LOOP_MODE_LOOP = 1,
    };

    u32 typeInfo;                 // 0x0
    u32 signature;                // 0x4
    ut::Offset name;              // 0x8
    ut::Offset m_Unknown0xC;      // 0xC
    u8 loopMode;                  // 0x10 LoopMode
    u8 m_Unknown0x11[3];          // 0x11
    f32 frameSize;                // 0x14 the last frame
    s32 memberAnimCount;          // 0x18
    ut::Offset memberAnimDic;     // 0x1C ResDicPatricia of the ResMemberAnimData
    s32 userDataCount;            // 0x20
    ut::Offset userDataDic;       // 0x24
};
ASSERT_OFFSET(ResAnimData, loopMode, 0x10);
ASSERT_SIZE(ResAnimData, 0x28);
ASSERT_OFFSET(ResAnimData, memberAnimDic, 0x1C);

// the binary resource of an animation (the wrapper of its data)
class ResAnim
{
public:
    // (inline)
    explicit ResAnim(const void* pData = nullptr) : m_pData(static_cast<const ResAnimData*>(pData)) {}
    const ResAnimData* ptr() const { return m_pData; }
    bool IsValid() const { return m_pData != nullptr; }
    u8 GetLoopMode() const { return m_pData->loopMode; }
    f32 GetFrameSize() const { return m_pData->frameSize; }
    s32 GetMemberAnimCount() const { return m_pData->memberAnimCount; }
    // the member animation at index (the order of the dictionary)
    ResMemberAnim GetMemberAnim(int index) const
    {
        ut::ResDicPatricia dic = {m_pData->memberAnimDic.To<ut::ResDicPatricia::Data>()};
        return ResMemberAnim(dic.GetDataByIndex(index));
    }

    // the textures of the texture pattern animations forgotten
    void Cleanup(); // 0x001385DC | nintendogs:bytes [tier A]
    // the texture pattern animations resolved
    u32 Setup(os::IAllocator* pAllocator, gfx::res::ResGraphicsFile graphicsFile); // 0x004D6124 | nintendogs:bytes [tier A]

    // A copy of the animation for the game: the header and the dictionary are copied (names and
    // member animations stay in the source), the texture pattern animations with their texture
    // references, which the copy binds itself (names are ours).
    static ResAnim Clone(os::IAllocator* pAllocator, const ResAnim& source); // 0x004D5850
    static void DestroyClone(os::IAllocator* pAllocator, const ResAnim& anim); // 0x004D5E5C

    const ResAnimData* m_pData; // 0x0
};
ASSERT_SIZE(ResAnim, 0x4);
} // namespace res
} // namespace anim
} // namespace nw
