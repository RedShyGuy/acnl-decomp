// The checksum of the villager data is Thumb code (built with -mthumb -mfloat-abi=softfp, see CMakeLists.txt).
asm(".eabi_attribute Tag_ABI_VFP_args, 3");

#include "Sv/dSvChecksum.h"
#include "Sv/dSvNpc.h"

// 0x00119870 (name is ours)
void SvNpcData::UpdateChecksum()
{
    checksum = SvCalcChecksum(&checksum + 1, 0x22BC8);
}

// 0x0011988C (name is ours)
bool SvNpcData::IsChecksumValid(bool isLogged)
{
    (void)isLogged;
    return checksum == SvCalcChecksum(&checksum + 1, 0x22BC8);
}
