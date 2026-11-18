#pragma once

#include "decomp.h"
#include "nn/gx/gx_Types.h"
#include "ssys/st/dListNode.h"

namespace cfl {
// an icon of a Mii: drawn into its texture by the Mii module (cfl/dCfl.h) after AddIcon (class name from the RTTI; the
// member and function names are ours)
// RTTI N3cfl7MiiIconE @ 0x008D0C58
// vtable 0x00903914 (vptr 0x0090391C), offset_to_top 0, 2 entries
class MiiIcon : public ::ssys::st::ListNode
{
public:
    MiiIcon(); // 0x004E1084
    virtual ~MiiIcon() override;
    // 0x004E10C8 (deleting dtor)

    // the icon of a Mii given by its data (CFL source 5)
    void Set(GLuint texture, GLenum format, u16 width, u16 height, const void* data); // 0x004E0FE8
    // the icon of the Mii index of the database (CFL source 0)
    void Set(GLuint texture, GLenum format, u16 width, u16 height, u16 index); // 0x004E1020

    GLuint m_Texture;     // 0x0C
    const void* m_pData;  // 0x10
    s32 m_Source;         // 0x14 (CFL_MakeIcon)
    GLenum m_Format;      // 0x18
    u16 m_Width;          // 0x1C
    u16 m_Height;         // 0x1E
    u16 m_Index;          // 0x20
};
ASSERT_SIZE(MiiIcon, 0x24);
} // namespace cfl
