#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"
#include "bg/bg_Extern.h"
#include "bg/dBgModelMgr.h"
#include "bg/dCharRoom.h"
#include "g3d/dResourceLoader.h"
#include "mutil/dBaseResourceMgr.h"
#include "nn/fnd/fnd_DateTime.h"
#include "nn/math/math_Vector3.h"
#include "ssys/ma/dHeapAllocator.h"

// The sunshine of the room (an object of the module ModuleIndoor with the model "sunshine" of the ground file, on the
// east or west wall of some rooms, depending on the date; no RTTI, layout unknown, names are ours). Its code is in the
// module; code.bin reaches it only through the import thunks of static.crs at 0x001500F0.. The addresses are the thunks;
// the comments name the code in the module.
class CharRoomSunshine
{
public:
    CharRoomSunshine(); // 0x00150130 (ModuleIndoor +0x0560B0)
    ~CharRoomSunshine(); // 0x00150138 (ModuleIndoor +0x0560EC)

    void Initialize(const void* model, g3d::ResourceLoader* textures, ssys::ma::HeapAllocator* allocator,
                    const nn::math::VEC3* position, const nn::math::VEC3* scale, const s16* angle,
                    f32 height); // 0x001500F0 (ModuleIndoor +0x055CE4)
    void Finalize(); // 0x001500F8 (ModuleIndoor +0x05608C)
    void SetColor(const u8* color, u8 value); // 0x00150100 (ModuleIndoor +0x055CA0)
    void Draw(); // 0x00150120 (ModuleIndoor +0x055CD8)
    void Calc(const nn::fnd::DateTimeParameters* date, const nn::math::VEC3* position,
              const s16* angle); // 0x00150128 (ModuleIndoor +0x055F98)

    // the date tests: whether the sunshine is shown at all, and which of the two places it takes (meaning of A/B unknown)
    static bool IsShown(const nn::fnd::DateTimeParameters* date); // 0x00150108 (ModuleIndoor +0x03588C)
    static bool IsSideA(const nn::fnd::DateTimeParameters* date); // 0x00150110 (ModuleIndoor +0x00C000)
    static bool IsSideB(const nn::fnd::DateTimeParameters* date); // 0x00150118 (ModuleIndoor +0x00BF54)

    u8 m_Data[0x68]; // (layout unknown)
};
ASSERT_SIZE(CharRoomSunshine, 0x68);

// The view of the rooms of the characters (the player houses, the houses of the villagers, the museum rooms, the
// island huts ...): the models of the ground of the room (BgModelMgr), the room textures, the floor, the walls and the
// landings with the wallpaper and the flooring of the room, and the sunshine (an object of ModuleIndoor). Only the class
// name is from the RTTI; all other names are ours.
// RTTI 17BsCharRoomViewMgr @ 0x008CC468
// vtable 0x008F3438 (vptr 0x008F3440), offset_to_top 0, 22 entries
class BsCharRoomViewMgr : public ::UtlBase<Base>
{
public:
    // the frames between two updates of the light colors
    static const u32 LIGHT_UPDATE_FRAMES = 1800;

    BsCharRoomViewMgr(); // 0x002C6FAC
    virtual ~BsCharRoomViewMgr(); // 0x002C70D8 slot 0x00
    // 0x002C70C8 slot 0x04 (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x002C6A84 slot 0x24
    virtual oml::framework::Result Draw(); // 0x002C6898 slot 0x30
    bool StartInitialize() override; // 0x002C5C34 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x002C58EC slot 0x44
    bool EndInitialize() override; // 0x002C56C4 slot 0x48
    bool StartFinalize() override; // 0x002C645C slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x002C6288 slot 0x50
    bool EndFinalize() override; // 0x002C60AC slot 0x54

    // the floor rectangle of the room (from the bones pos_wallN/E/W/S of the ground model)
    void GetRoomRect(f32* minX, f32* minZ, f32* maxX, f32* maxZ); // 0x002C5FA8
    // the colors of the windows and of the sunshine from the light colors
    void UpdateLights(); // 0x002C67B4

    // a new wallpaper / flooring of the room of the player (or of a museum room) with its value; the other players get
    // it as packet 120 / 121 (false: not in an own house or not a wallpaper / flooring)
    static bool SetWallpaper(const SvFgName* item, bool playSound); // 0x002C60B4
    static bool SetWallpaper(const SvFgName* item, u8 param, bool playSound); // 0x002C6614
    static bool SetFlooring(const SvFgName* item, bool playSound); // 0x002C6DD8
    static bool SetFlooring(const SvFgName* item, u8 param, bool playSound); // 0x002C6474

    static BsCharRoomViewMgr* s_pInstance;

private:
    // (inline) the fixed wallpaper and flooring of the island huts and the chip tutorial
    DECOMP_ALWAYS_INLINE void SetFixedCoverings();

public:
    bool m_IsResourceReady;                                         // 0x14
    bool m_IsFloorReady;                                            // 0x15
    bool m_IsWallReady;                                             // 0x16
    bool m_IsLandingReady;                                          // 0x17
    ssys::ma::HeapAllocator m_Allocator;                            // 0x18 "BsCharRoomViewMgr"
    u16 m_BgName;                                                   // 0x20 the ground of block 0/0 of the field
    RoomCovering m_Wallpaper;                                       // 0x22
    RoomCovering m_Flooring;                                        // 0x28
    u32 m_LightTimer;                                               // 0x30
    mutil::BaseResourceMgr<g3d::ResourceLoader, 2u> m_ResourceMgr; // 0x34
    mutil::ResourceKey m_RoomTexturesKey;                           // 0x298 "Bg/Indoor/RoomTextures/room.bcres"
    mutil::ResourceKey m_CommonKey;                                 // 0x2A0 "Bg/Indoor/Common_idr/Common_idr.bcres"
    BgModelMgr m_BgModels;                                          // 0x2A8
    CharRoomSunshine m_Sunshine;                                // 0x3538
    CharRoomFloor m_Floor;                                          // 0x35A0
    CharRoomWall m_Wall;                                            // 0x3850
    CharRoomLanding m_Landing;                                      // 0x3BE8
    nn::math::VEC3 m_WallEPosition;                                 // 0x3F48 the bone pos_wallE
    nn::math::VEC3 m_WallWPosition;                                 // 0x3F54 the bone pos_wallW
};
ASSERT_OFFSET(BsCharRoomViewMgr, m_IsResourceReady, 0x14);
ASSERT_OFFSET(BsCharRoomViewMgr, m_ResourceMgr, 0x34);
ASSERT_OFFSET(BsCharRoomViewMgr, m_RoomTexturesKey, 0x298);
ASSERT_OFFSET(BsCharRoomViewMgr, m_BgModels, 0x2A8);
ASSERT_OFFSET(BsCharRoomViewMgr, m_Sunshine, 0x3538);
ASSERT_OFFSET(BsCharRoomViewMgr, m_Floor, 0x35A0);
ASSERT_OFFSET(BsCharRoomViewMgr, m_Wall, 0x3850);
ASSERT_OFFSET(BsCharRoomViewMgr, m_Landing, 0x3BE8);
ASSERT_SIZE(BsCharRoomViewMgr, 0x3F60);
