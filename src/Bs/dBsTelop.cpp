#include "state/dMode.h"
#include "Other/dBase.h"
#include "Bs/dBsTelop.h"

// ctor address unknown
BsTelop::BsTelop()
{
}

// 0x0060CC50 slot 0x00 | slot vf_0x00 of oml::framework::Process
BsTelop::~BsTelop()
{
}

// 0x0060C37C slot 0x0C | slot vf_0x0C of oml::framework::Process
void BsTelop::Initialize()
{
}

// 0x0060CA24 slot 0x18 | slot vf_0x18 of oml::framework::Process
void BsTelop::Finalize()
{
}

// 0x0060C734 slot 0x24 | slot vf_0x24 of oml::framework::Process
void BsTelop::Calc()
{
}

// 0x0060C35C slot 0x30 | slot vf_0x30 of oml::framework::Process
void BsTelop::Draw()
{
}

// 0x0060BBC8 | libgarden [tier A]
void BsTelop::ShowTourTelop(BsTelop::TelopName, TourName)
{
}

// 0x0060C900 | libgarden [tier A]
void BsTelop::ShowTelop(BsTelop::TelopName)
{
}

