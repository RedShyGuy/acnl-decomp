#include "sead/seadTaskClassID.h"

namespace sead {
// 0x00982D88
TaskClassID::StringFactoryFunc TaskClassID::s_StringFactory = nullptr;
// 0x00982D8C
TaskClassID::IntFactoryFunc TaskClassID::s_IntFactory = nullptr;
} // namespace sead
