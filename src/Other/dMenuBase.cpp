#include "Other/dMenuBase.h"

// 0x006AB39C slot 0x10 (name after oml::framework::Process)
void MenuBase::HandleInitializationResult(oml::framework::Result result)
{
    Base::HandleInitializationResult(result);
}

// 0x006AB3A0 slot 0x1C (name after libgarden)
void MenuBase::HandleFinalizationResult(oml::framework::Result result)
{
    RemoveFromMenuList(&m_List);
    Base::HandleFinalizationResult(result);
}

// 0x006AB3C4 slot 0x20 (name after libgarden)
bool MenuBase::CanCalc() const
{
    return Base::CanCalc();
}

// 0x006AB3D8 slot 0x40 (name after libgarden)
void MenuBase::FUN_006ab3d8()
{
    // empty (in the original too)
}

// 0x006AB3DC slot 0x28 (name after oml::framework::Process)
void MenuBase::HandleCalcResult(oml::framework::Result result)
{
    Base::HandleCalcResult(result);
}

// 0x006AB3E0 slot 0x50 (name after libgarden)
void MenuBase::FUN_006ab3e0(bool value)
{
    unk0 = value;
}

// 0x006AB3E8 slot 0x44 (name after libgarden)
void MenuBase::OnClose()
{
    // empty (in the original too)
}

// 0x006AB3EC slot 0x5C (name after libgarden)
bool MenuBase::FUN_006ab3ec()
{
    return unk2;
}

// 0x006AB3F4 slot 0x48 (name after libgarden)
void MenuBase::FUN_006ab3f4(bool value)
{
    unk1 = value;
}

// 0x006AB3FC slot 0x58 (name after libgarden)
void MenuBase::FUN_006ab3fc(bool value)
{
    unk2 = value;
}

// 0x006AB40C slot 0x30 (name after oml::framework::Process)
oml::framework::Result MenuBase::Draw()
{
    return oml::framework::RESULT_SUCCESS;
}

// 0x006AB414 slot 0x0C (name after oml::framework::Process)
oml::framework::Result MenuBase::Initialize()
{
    return oml::framework::RESULT_SUCCESS;
}

// 0x006AB440 slot 0x54 (name after libgarden)
bool MenuBase::FUN_006ab440()
{
    return unk0;
}

// 0x006AB448 slot 0x24 (name after oml::framework::Process)
oml::framework::Result MenuBase::Calc()
{
    return oml::framework::RESULT_SUCCESS;
}

// 0x006AB450 slot 0x4C (name after libgarden)
bool MenuBase::FUN_006ab450()
{
    return unk1;
}

// 0x006AB458 slot 0x18 (name after oml::framework::Process)
oml::framework::Result MenuBase::Finalize()
{
    return oml::framework::RESULT_SUCCESS;
}

// 0x006AB460 slot 0x08 (name after libgarden)
bool MenuBase::CanInitialize() const
{
    if (!Base::CanInitialize()) {
        return false;
    }
    MenuBase* self = const_cast<MenuBase*>(this);
    AddToMenuList(&self->m_List);
    self->m_List.m_pMenu = self;
    self->m_Kind = m_Arg;
    return true;
}

// 0x006AB494 slot 0x14 (name after libgarden)
bool MenuBase::CanFinalize() const
{
    return Base::CanFinalize();
}

// 0x006AB4A8 (name after libgarden)
MenuBase::MenuBase() : m_Kind(0), unk0(false), unk1(false), unk2(false)
{
}

// 0x006AB508 slot 0x00 (name after libgarden)
// 0x006AB4E8 (deleting dtor)
MenuBase::~MenuBase()
{
    // (only the members and the base are destroyed)
}

// 0x00767B30 slot 0x60 (name after libgarden)
bool MenuBase::FUN_00767b30()
{
    return true;
}
