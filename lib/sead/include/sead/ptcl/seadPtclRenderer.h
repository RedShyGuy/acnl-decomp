#pragma once

#include "decomp.h"

namespace sead {
namespace ptcl {
class PtclRenderer
{
public:
    void beginRender(unsigned*, const sead::Matrix44<float>&, const sead::Matrix34<float>&, const sead::Matrix34<float>&); // 0x0055630C | mk7dlp:bytes [tier B]
    void beginStripeCalc(unsigned*); // 0x00556884 | mk7dlp:bytes [tier B]
    void endCalc(unsigned*); // 0x00556B40 | mk7dlp:bytes [tier B]
    void beginCalc(unsigned*); // 0x00556B7C | mk7dlp:bytes [tier B]
    void endRender(unsigned*); // 0x00556C18 | mk7dlp:bytes [tier B]
};
} // namespace ptcl
} // namespace sead
