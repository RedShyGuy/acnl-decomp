#pragma once

#include "decomp.h"
#include "Sv/dSvFgName.h"

namespace oml {
namespace framework {
class Process;
} // namespace framework
} // namespace oml

// The furniture actor of the item that is selected in a list (OrderList, TicketList): Show creates it (walls, carpets
// and paper as a placeholder wallpaper), a different item or Hide stops it. The object is a global of the file of
// BsFtrMgr (0x00950338); names are ours.
struct FtrPreview
{
    FtrPreview() : m_pProcess(NULL) { m_Name = SvFgName::s_Empty; }
    ~FtrPreview(); // 0x006925F8

    bool Show(const SvFgName& name); // 0x00692330
    bool Hide(); // 0x006925C0

    oml::framework::Process* m_pProcess; // 0x0
    SvFgName m_Name;                     // 0x4
};
ASSERT_SIZE(FtrPreview, 0x8);

FtrPreview* GetFtrPreview(); // 0x00692FD0
