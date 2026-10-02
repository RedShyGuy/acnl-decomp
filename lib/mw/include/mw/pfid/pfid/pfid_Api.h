#pragma once

#include "decomp.h"

namespace mw {
namespace pfid {
namespace pfid {
void FaceDetection(unsigned char*, short, short, short, short, mw::pfid::pfid::t_FaceDetect*); // 0x001261E0 | nintendogs:bytes [tier B]
void FaceGetFeature(unsigned char*, short, short, short, mw::pfid::pfid::t_FacePosition*, short*, unsigned char*); // 0x002D37E4 | nintendogs:bytes [tier B]
} // namespace pfid
} // namespace pfid
} // namespace mw
