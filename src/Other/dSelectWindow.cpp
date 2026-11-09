#include "state/dMode.h"
#include "Other/dSelectWindow.h"

// 0x0020A254 (name after the RTTI)
SelectWindow::SelectWindow()
{
}

// 0x0020A454 slot 0x00 (name after the RTTI)
// 0x0020A3A8 (deleting dtor)
SelectWindow::~SelectWindow()
{
}

// 0x0082AD88 slot 0x08
const char* SelectWindow::GetName(Func func) const
{
    return FindName(func);
}

// 0x00209C48 (name is ours)
void SelectWindow::Release()
{
}

// 0x00209914 (name is ours)
bool SelectWindow::Setup(u32)
{
}

// 0x00209C80 (name is ours)
void SelectWindow::Update()
{
}

// 0x00208E64 (name is ours)
void SelectWindow::Open(s32)
{
}

// 0x0020A18C (name is ours)
void SelectWindow::OpenWithCancel(s32)
{
}

// 0x002082FC (name is ours)
void SelectWindow::Close()
{
}

// 0x00209464 (name is ours)
void SelectWindow::State_Select()
{
}

// 0x00208CD0 (name is ours)
void SelectWindow::State_Close()
{
}

// 0x00818154 (name is ours)
template void state::Mode<SelectWindow>::Change(Func func);

// 0x007131DC (name is ours)
bool SelectWindow::IsClosed()
{
}

// 0x0071322C (name is ours)
bool SelectWindow::IsBusy()
{
}
