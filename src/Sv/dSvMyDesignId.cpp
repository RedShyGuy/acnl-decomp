#include "Sv/dSvMyDesign.h"

#include <cstring>
#include "Item/dResLoader.h"
#include "Other/dTextureImage.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvMuseum.h"
#include "Sv/dSvNpc.h"
#include "Sv/dSvPlayer.h"
#include "Sv/dSvTown.h"
#include "field/field_Extern.h"
#include "netgame/dNetGameMgr.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "nw/ut/ut_LinkList.h"
#include "script/dScriptExtern.h"

// The designs in the save data by their id and the textures of the item loaders that show them (the file after MenuBase;
// all names are ours).

// the item loaders that show a design (RegisterDesignLoader)
// 0x00A22D44
nw::ut::LinkList<item::ResLoader, offsetof(item::ResLoader, m_DesignNode)> g_DesignLoaders;

namespace {
// the kinds of the design ids
const u32 DESIGN_KIND_PLAYER = 0;
const u32 DESIGN_KIND_VILLAGER = 1;
const u32 DESIGN_KIND_DISPLAY = 2;
const u32 DESIGN_KIND_RETAIL = 3;
const u32 DESIGN_KIND_EXHIBIT = 4;
const u32 DESIGN_KIND_TOWN_FLAG = 5;
const u32 DESIGN_KIND_STAND = 6;
// the bits of the second id of network packet 109
const u16 PACKET_FLAG_SWAP = 0x8000;
const u16 PACKET_FLAG_RESET = 0x4000;
// the height of a large texture of a design (128: four rows of parts)
const u32 LARGE_TEXTURE_HEIGHT = 128;
const u32 LARGE_TEXTURE_SIZE = 0x2000;
const u32 SMALL_TEXTURE_SIZE = 0x800;

// the first texture of the graphics file of a loader
inline const void* GetLoaderTexture(const item::ResLoader* loader)
{
    return loader->m_GraphicsFile.GetTextures(0);
}

// the pixels of an image texture
inline void* GetTexturePixels(const void* texture)
{
    const nw::gfx::res::ResImageTextureData* image =
        nw::gfx::res::DynamicCastData<nw::gfx::res::ResImageTextureData>(texture, nw::gfx::res::ResTexture::TYPE_IMAGE);
    return const_cast<void*>(image->image.To<nw::gfx::res::ResImageData>()->data.ToPtr());
}
} // namespace

// 0x006AB518
void RegisterDesignLoader(item::ResLoader* loader)
{
    g_DesignLoaders.PushBack(loader);
}

// 0x006AB530
void DrawDesignPixel(SvMyDesign* design, u16 color, u32 x, u32 y)
{
    for (nw::ut::internal::LinkListImpl::iterator it = g_DesignLoaders.begin(); it != g_DesignLoaders.end(); ++it) {
        item::ResLoader* loader = g_DesignLoaders.GetPointerFromNode(it.GetNode());
        if (loader->m_pDesign == design) {
            design->DrawPixel(GetLoaderTexture(loader), color, x, y);
        }
    }
}

// 0x006AB5C0
SvMyDesign* GetDesign(u16 id)
{
    const u32 index = id & 15;
    const u32 sub = (id >> 4) & 15;
    switch ((id >> 8) & 15) {
    case DESIGN_KIND_PLAYER:
        return static_cast<SvMyDesign*>(Func20C50C(SvGetPlayer(static_cast<PlayerSaveNumber>(sub))->patterns, index));
    case DESIGN_KIND_VILLAGER:
        return &Func305AC0(GetGardenData()->GetNpcData()->npcs, sub)->pattern;
    case DESIGN_KIND_DISPLAY:
        return GetGardenData()->townData.unk_0xEE2C.unk_0xEF14.GetDesign(index);
    case DESIGN_KIND_RETAIL:
        return &GetGardenData()->townData.unk_0xEE2C.reTailItemDesign[index & 7];
    case DESIGN_KIND_EXHIBIT:
        return GetGardenData()->townData.museum.GetExhibit(sub)->GetPoster();
    case DESIGN_KIND_STAND:
        return &GetGardenData()->strcData.stands[index].pattern;
    case DESIGN_KIND_TOWN_FLAG:
        return &GetGardenData()->townData.townFlag;
    }
    return NULL;
}

// 0x006AB6F0
void UnregisterDesignLoader(item::ResLoader* loader)
{
    for (nw::ut::internal::LinkListImpl::iterator it = g_DesignLoaders.begin(); it != g_DesignLoaders.end(); ++it) {
        if (g_DesignLoaders.GetPointerFromNode(it.GetNode()) == loader) {
            g_DesignLoaders.Erase(loader);
            return;
        }
    }
}

// 0x006AB738
u16 FindDesignId(const SvMyDesign* design)
{
    for (u16 player = 0; player < 8; player++) {
        SvPlayer* svPlayer = SvGetPlayer(static_cast<PlayerSaveNumber>(player & 0xFF));
        if (svPlayer == NULL) {
            continue;
        }
        for (u16 i = 0; i < 10; i++) {
            if (Func20C50C(svPlayer->patterns, i) == design) {
                return (DESIGN_KIND_PLAYER << 8) | (player << 4) | i;
            }
        }
    }
    SvTownDataUnkEF14* displays = &GetGardenData()->townData.unk_0xEE2C.unk_0xEF14;
    for (u16 i = 0; i < 8; i++) {
        if (displays->GetDesign(i) == design) {
            return (DESIGN_KIND_DISPLAY << 8) | i;
        }
    }
    SvMyDesign* retail = GetGardenData()->townData.unk_0xEE2C.reTailItemDesign;
    for (u16 i = 0; i < 8; i++) {
        if (&retail[i & 7] == design) {
            return (DESIGN_KIND_RETAIL << 8) | i;
        }
    }
    SvDesignStand* stands = GetGardenData()->strcData.stands;
    for (u16 i = 0; i < 8; i++) {
        if (&stands[i].pattern == design) {
            return (DESIGN_KIND_STAND << 8) | i;
        }
    }
    for (u16 i = 0; i < 10; i++) {
        SvNpc* npc = Func305AC0(GetGardenData()->GetNpcData()->npcs, i);
        if (npc != NULL && &npc->pattern == design) {
            return (DESIGN_KIND_VILLAGER << 8) | (i << 4);
        }
    }
    for (u32 i = 0; i < 4; i++) {
        if (GetGardenData()->townData.museum.GetExhibit(i & 0xFF)->GetPoster() == design) {
            return (DESIGN_KIND_EXHIBIT << 8) | (i << 4);
        }
    }
    if (&GetGardenData()->townData.townFlag == design) {
        return DESIGN_KIND_TOWN_FLAG << 8;
    }
    return 0xFFFF;
}

// 0x006AB940
void AssignDesign(SvMyDesign* design, const SvMyDesign* src)
{
    std::memcpy(static_cast<void*>(design), src, sizeof(SvMyDesign));
    UpdateDesignTextures(design);
}

// 0x006AB95C
void SwapDesigns(SvMyDesign* a, SvMyDesign* b)
{
    SvMyDesign tmp;
    tmp.CopyFrom(a);
    std::memcpy(static_cast<void*>(a), b, sizeof(SvMyDesign));
    std::memcpy(static_cast<void*>(b), &tmp, sizeof(SvMyDesign));
    UpdateDesignTextures(a);
    UpdateDesignTextures(b);
}

namespace netgame {
// network packet 109: a design was copied, swapped (bit 15 of the second id) or made new (bit 14)
// 0x006AB9BC
void PacketHandler6AB9BC(u32 /* sender */, const void* data, u32 /* size */, u32 /* target */, u32 /* stage */)
{
    if (data == NULL) {
        return;
    }
    const u32 value = *static_cast<const u32*>(data);
    const u16 first = value & 0xFFFF;
    const u16 second = value >> 16;
    if (second & PACKET_FLAG_SWAP) {
        SwapDesigns(GetDesign(first), GetDesign(second & 0xFFF));
    } else if (second & PACKET_FLAG_RESET) {
        SvMyDesign* design = GetDesign(second & 0xFFF);
        if (design != NULL) {
            design->InitNewForNetwork();
        }
    } else {
        AssignDesign(GetDesign(first), GetDesign(second & 0xFFF));
        OnDesignChanged(first, true);
        return;
    }
    OnDesignChanged(first, true);
    OnDesignChanged(second & 0xFFF, true);
}
} // namespace netgame

// 0x006ABA70
void OnDesignChanged(u16 id, bool isFromNetwork)
{
    const u32 index = id & 15;
    const u32 sub = (id >> 4) & 15;
    if (((id >> 8) & 15) != DESIGN_KIND_PLAYER) {
        return;
    }
    netgame::GetDefaultSaveNo(4); // (the result is not used)
    if (SvIsValidPlayerSaveNo(static_cast<PlayerSaveNumber>(sub))) {
        Func24F5CC(sub, index, true);
    } else {
        Func24F5CC(sub, index, false);
    }
    Func4E638C(sub, index, !isFromNetwork);
}

// 0x006ABAE4
bool Func6ABAE4()
{
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL) {
        return false;
    }
    SvMyDesign* designs = player->patterns;
    if (!Func7133CC(designs)) {
        return false;
    }
    u8 bits[2] = {0, 0};
    for (u32 i = 0; i < 10; i++) {
        if (static_cast<SvMyDesign*>(Func20C4C8(designs, i))->isGuestDesign && i < 10) {
            bits[i >> 3] |= 1 << (i & 7);
        }
    }
    Func20C524(designs);
    netgame::GetDefaultSaveNo(4);
    Func4E514C(bits);
    return true;
}

// 0x006ABB84
void UpdateDesignTexturesEntry(SvMyDesign* design)
{
    UpdateDesignTextures(design);
}

// 0x006ABB88
void UpdateDesignTextures(SvMyDesign* design)
{
    const void* source = NULL;
    bool isLargeDone = false;
    for (nw::ut::internal::LinkListImpl::iterator it = g_DesignLoaders.begin(); it != g_DesignLoaders.end(); ++it) {
        item::ResLoader* loader = g_DesignLoaders.GetPointerFromNode(it.GetNode());
        if (loader->m_pDesign != design) {
            continue;
        }
        const void* texture = GetLoaderTexture(loader);
        const nw::gfx::res::ResImageTextureData* image =
            nw::gfx::res::DynamicCastData<nw::gfx::res::ResImageTextureData>(texture, nw::gfx::res::ResTexture::TYPE_IMAGE);
        if (image->height == LARGE_TEXTURE_HEIGHT) {
            if (isLargeDone && source != NULL) {
                std::memcpy(GetTexturePixels(texture), source, LARGE_TEXTURE_SIZE);
            } else {
                design->DrawToTextureByPairs(texture, false);
                isLargeDone = true;
                source = GetTexturePixels(texture);
            }
        } else if (source != NULL) {
            std::memcpy(GetTexturePixels(texture), source, SMALL_TEXTURE_SIZE);
        } else {
            design->DrawToTextureByPairs(texture, true);
            source = GetTexturePixels(texture);
        }
    }
}
