#pragma once

#include "decomp.h"
#include "Other/dDragIconNode.h"

// RTTI 17PartsDragIconNode @ 0x008CC694
// vtable 0x008F3B38 (vptr 0x008F3B40), offset_to_top 0, 29 entries
class PartsDragIconNode : public ::DragIconNode
{
public:
    PartsDragIconNode(); // ctor candidate(s) 0x002D0688 (unverified)
    virtual ~PartsDragIconNode(); // 0x001FFF90 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x002D06A0 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    nw::lyt::Pane* GetIconPane() override; // 0x0072032C slot 0x40
    void SetAttached(bool attach) override; // 0x002D0680 slot 0x44
    void Release() override; // 0x002D0684 slot 0x54
    void DrawDragged() override; // 0x002D067C slot 0x58
    void SetParentPane(nw::lyt::Pane* pane) override; // 0x002D05EC slot 0x5C
    nw::lyt::Pane* GetParentPane() override; // 0x002D05E4 slot 0x60
    void OnDropTargetEnter() override; // 0x002D05F4 slot 0x64
    void OnDropTargetLeave() override; // 0x002D0648 slot 0x68
    bool IsNotDraggable() override; // 0x00720334 slot 0x6C
    f32 GetPan() override; // 0x0072033C slot 0x70
};
