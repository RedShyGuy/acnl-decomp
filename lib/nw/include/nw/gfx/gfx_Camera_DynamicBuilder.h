#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_Camera.h"

// Builds a camera without a resource of the file: the updaters set (else a look-at view and a
// perspective projection with their defaults) and a resource of its own (the class name and Create
// are from the reference symbols, the members are ours).
class nw::gfx::Camera::DynamicBuilder
{
public:
    // the camera (NULL: no memory)
    Camera* Create(os::IAllocator* pAllocator); // 0x004AD2A4 | nintendogs:bytes [tier A]

    Description m_Description;                          // 0x00
    GfxPtr<CameraViewUpdater> m_ViewUpdater;            // 0x10
    GfxPtr<CameraProjectionUpdater> m_ProjectionUpdater; // 0x18
};
ASSERT_SIZE(nw::gfx::Camera::DynamicBuilder, 0x20);
