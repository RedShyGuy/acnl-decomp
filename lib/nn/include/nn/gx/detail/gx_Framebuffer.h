#pragma once

// The framebuffer manager of the GL functions (__fb_*): a list of framebuffers with a color and a
// depth attachment. All names are ours except those of the __fb_ functions.

#include "decomp.h"
#include "nn/gx/detail/gx_Surface.h"

namespace nn {
namespace gx {
namespace detail {
const u32 ATTACHMENT_COLOR = 0;
const u32 ATTACHMENT_DEPTH = 1;
const u32 ATTACHMENT_COUNT = 2;

struct FramebufferAttachment
{
    u32 _unknown0x0[3];
    Surface* m_pSurface;  // 0xC
};
ASSERT_SIZE(FramebufferAttachment, 0x10);

struct Framebuffer
{
    FramebufferAttachment m_Attachments[ATTACHMENT_COUNT]; // 0x00
    u32 m_Unknown0x20;                                     // 0x20
    Framebuffer* m_pNext;                                  // 0x24
};
ASSERT_SIZE(Framebuffer, 0x28);

// the manager (in the memory of the GL state)
struct FramebufferManager
{
    Framebuffer* m_pFramebuffers;  // 0x0 (the default one first)
    u32 m_Unknown0x4;              // 0x4
    Framebuffer* m_pCurrent;       // 0x8
    u32 m_Unknown0xC;              // 0xC
};
ASSERT_SIZE(FramebufferManager, 0x10);

extern FramebufferManager* s_pFramebufferManager;

// a new framebuffer without attachments
Framebuffer* CreateFramebuffer(); // 0x0012E4E4 (name is ours)
} // namespace detail
} // namespace gx
} // namespace nn

extern "C" {
int __fb_initializeFBManager(void* pMemory);
nn::gx::detail::Surface* __fb_getCurrentFBColorSurface();
nn::gx::detail::Surface* __fb_getCurrentFBDepthSurface();
// the surface leaves all framebuffers
void __fb_detachSurfaceContainer(nn::gx::detail::Surface* pSurface);
}
