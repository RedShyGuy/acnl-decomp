#include "state/dMode.h"
#include "Item/dItemSelectWindow.h"

// 0x002BB0CC (name after the RTTI)
ItemSelectWindow::ItemSelectWindow()
{
}

// 0x002BB248 slot 0x00 (name after the RTTI)
// 0x002BB1D8 (deleting dtor)
ItemSelectWindow::~ItemSelectWindow()
{
}

// 0x0082BD00 slot 0x08
const char* ItemSelectWindow::GetName(Func func) const
{
    return FindName(func);
}

// 0x002BAD54 (name is ours)
bool ItemSelectWindow::IsSetup()
{
}

// 0x002BA9BC (name is ours)
bool ItemSelectWindow::Setup(u32)
{
}

// 0x002BACB8 (name is ours)
void ItemSelectWindow::Update()
{
}

// 0x002BB054 (name is ours)
void ItemSelectWindow::Open(s32)
{
}

// 0x002BA3AC (name is ours)
void ItemSelectWindow::State_Close()
{
}

// 0x00819D80 (name is ours)
template void state::Mode<ItemSelectWindow>::Change(Func func);

// 0x0071DD70 (name is ours)
bool ItemSelectWindow::IsClosed()
{
}

// 0x0071DDC0 (name is ours)
bool ItemSelectWindow::IsBusy()
{
}
