#pragma once

#include "decomp.h"
#include "Exchange/dExchangeBankKeywordCampingCarTexture.h"
#include "Exchange/dExchangeResBank.h"
#include "g3d/dResourceLoader.h"

// vtable +0x67270 in ModuleIndoor.cro, offset_to_top 0, 8 entries
class CampingCarTextureBank : public ::ExchangeResBank<153600u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordCampingCarTexture, g3d::ResourceLoader, 1u, false>
{
public:
    CampingCarTextureBank(); // ctor address unknown
};
