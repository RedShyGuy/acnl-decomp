#pragma once

#include "decomp.h"

class CFLiTexHandle
{
public:
    void CreateFromRes(const CFLiTexHeader*, unsigned); // 0x0022BD8C | nintendogs:bytes [tier B]
    void Create(int, int, int, CFLTexWrap, CFLTexWrap, CFLTexFmt, const void*, unsigned, unsigned char); // 0x0022BDDC | nintendogs:bytes [tier B]
    void Delete(); // 0x0022BFA4 | nintendogs:bytes [tier B]
};
