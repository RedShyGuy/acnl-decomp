#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/os/os_IAllocator.h"

namespace nw {
namespace gfx {
namespace res {
// 0x0013B920 | nintendogs:bytes [tier A]
void nw::gfx::res::ResMesh::Cleanup()
{
    ptr()->flags &= ~ResMeshData::FLAG_SETUP_DONE;
    if (ptr()->pCommand != nullptr) {
        ptr()->pAllocator->Free(ptr()->pCommand);
        ptr()->pCommand = nullptr;
        ptr()->commandSize = 0;
        // the deactivate commands were in the same memory
        if (ptr()->pDeactivateCommand != nullptr) {
            ptr()->pDeactivateCommand = nullptr;
            ptr()->deactivateCommandSize = 0;
        }
    }
}

// 0x004A9E64 | nintendogs:callgraph [tier A]
void nw::gfx::res::ResMesh::Setup(nw::gfx::res::ResModel, nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile)
{
}

} // namespace res
} // namespace gfx
} // namespace nw
