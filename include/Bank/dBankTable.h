#pragma once

#include "decomp.h"

// Instantiations found in the binary:
//   BankTable<Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>  typeinfo 0x008CD650  vtable 0x008FA158
//   BankTable<Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>  typeinfo 0x008CD65C  vtable 0x008FA180
//   BankTable<Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 50u>  typeinfo 0x008CD668  vtable 0x008FA1A8
//   BankTable<Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 7u>  typeinfo 0x008CD674  vtable 0x008FA1D0
//   BankTable<Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>  typeinfo 0x008CD680  vtable 0x008FA1F8
//   BankTable<Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >, 50u>  typeinfo 0x008CD68C  vtable 0x008FA220
//   BankTable<HumanResTextureAnimBank, 10u>  typeinfo 0x008CD638  vtable 0x008FA108
//   BankTable<HumanResTextureAnimBank, 14u>  typeinfo 0x008CD644  vtable 0x008FA130
//   BankTable<NpcTopsBank, 10u>  typeinfo 0x008CD5CC  vtable 0x008F9FA0
//   BankTable<PlayerAcceBank, 7u>  typeinfo 0x008CD5E4  vtable 0x008F9FF0
//   BankTable<PlayerBottomsBank, 7u>  typeinfo 0x008CD62C  vtable 0x008FA0E0
//   BankTable<PlayerCapBank, 7u>  typeinfo 0x008CD5D8  vtable 0x008F9FC8
//   BankTable<PlayerFaceBank, 14u>  typeinfo 0x008CD5F0  vtable 0x008FA018
//   BankTable<PlayerHeadBank, 7u>  typeinfo 0x008CD5FC  vtable 0x008FA040
//   BankTable<PlayerLegsBank, 7u>  typeinfo 0x008CD608  vtable 0x008FA068
//   BankTable<PlayerShoesBank, 7u>  typeinfo 0x008CD620  vtable 0x008FA0B8
//   BankTable<PlayerTopsBank, 7u>  typeinfo 0x008CD614  vtable 0x008FA090
//   BankTable<ToolBank, 10u>  typeinfo 0x008CD698  vtable 0x008FA248
//   BankTable<ToolBank, 7u>  typeinfo 0x008CD6A4  vtable 0x008FA270
template <typename T0, auto T1>
class BankTable
{
public:
    // TODO: members unknown
};
