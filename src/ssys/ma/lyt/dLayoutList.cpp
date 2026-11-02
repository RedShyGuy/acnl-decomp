#include "ssys/ma/lyt/dLayoutList.h"
#include "ssys/ma/lyt/dLayout.h"

namespace ssys {
namespace ma {
namespace lyt {
// 0x005684F8 (name is ours)
void ssys::ma::lyt::LayoutList::Draw(u32 drawMode)
{
    for (Base2D* node = static_cast<Base2D*>(m_pFirst); node != NULL; node = static_cast<Base2D*>(node->m_pNext)) {
        static_cast<Layout*>(node)->m_DrawMode = drawMode;
        node->DrawCached();
    }
}

// 0x00568530 (name is ours)
void ssys::ma::lyt::LayoutList::Add(Base2D* node)
{
    for (Base2D* it = static_cast<Base2D*>(m_pFirst); it != NULL; it = static_cast<Base2D*>(it->m_pNext)) {
        if (it->m_Layer > node->m_Layer) {
            Insert(it, node);
            return;
        }
    }
    PushBack(node);
}

// 0x00568568 slot 0x08 (name is ours)
void ssys::ma::lyt::LayoutList::DrawSave()
{
    for (Base2D* node = static_cast<Base2D*>(m_pFirst); node != NULL; node = static_cast<Base2D*>(node->m_pNext)) {
        node->DrawSave();
    }
}

// 0x0056859C slot 0x00
// 0x00568598 (deleting dtor)
ssys::ma::lyt::LayoutList::~LayoutList()
{
    // empty (in the original too)
}

} // namespace lyt
} // namespace ma
} // namespace ssys
