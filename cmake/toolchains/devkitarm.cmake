# Toolchain for non-matching builds with devkitARM (GCC).
#
#   cmake -B build/gcc -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/devkitarm.cmake
#
# The only toolchain of the project. Nintendo's ARMCC 4.1 is not used, so the output does not
# match the original binary byte for byte; tools/decomp/check.py compares it by structure.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

if(NOT DEVKITARM)
    set(DEVKITARM "$ENV{DEVKITARM}")
endif()
if(NOT DEVKITARM)
    if(EXISTS "C:/devkitPro/devkitARM")
        set(DEVKITARM "C:/devkitPro/devkitARM")
    else()
        set(DEVKITARM "/opt/devkitpro/devkitARM")
    endif()
endif()
# devkitPro's installer sets DEVKITARM=/opt/devkitpro/devkitARM (an MSYS path) on Windows
if(CMAKE_HOST_WIN32 AND NOT EXISTS "${DEVKITARM}/bin" AND EXISTS "C:/devkitPro/devkitARM/bin")
    set(DEVKITARM "C:/devkitPro/devkitARM")
endif()
set(DEVKITARM "${DEVKITARM}" CACHE PATH "devkitARM installation")
if(CMAKE_HOST_WIN32)
    set(_exe ".exe")
else()
    set(_exe "")
endif()

set(CMAKE_C_COMPILER "${DEVKITARM}/bin/arm-none-eabi-gcc${_exe}")
set(CMAKE_CXX_COMPILER "${DEVKITARM}/bin/arm-none-eabi-g++${_exe}")
set(CMAKE_AR "${DEVKITARM}/bin/arm-none-eabi-gcc-ar${_exe}")
set(CMAKE_RANLIB "${DEVKITARM}/bin/arm-none-eabi-gcc-ranlib${_exe}")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Options that keep GCC's code closer to the original (ARMCC's), so check.py compares like with like:
#   -fno-optimize-strlen, -fno-tree-loop-distribute-patterns: no malloc + memset -> calloc,
#       no loop -> memset / memcpy call (ARMCC never does either)
#   -fno-reorder-blocks: blocks stay in source order; otherwise GCC copies loop heads
#       (an ldrex / strex retry loop gets a second ldrex), which ARMCC does not
#   -fno-store-merging: two byte stores stay two stores (GCC would make one strh)
#   -fno-lifetime-dse: stores to members in a destructor stay (e.g. "mHandle = 0" after closing it)
#   -fno-tree-switch-conversion: a switch stays compares (GCC would make a lookup table in .rodata)
#   -fno-math-errno: sqrtf is a bare vsqrt (ARMCC does not set errno; GCC would add a call to sqrtf for NaN)
set(DECOMP_GCC_FLAGS
    "-march=armv6k -mtune=mpcore -mfloat-abi=hard -mfpu=vfp -marm -mtp=soft -O2 -ffunction-sections -fdata-sections -fno-optimize-strlen -fno-tree-loop-distribute-patterns -fno-reorder-blocks -fno-store-merging -fno-lifetime-dse -fno-tree-switch-conversion -fno-math-errno -w"
    CACHE STRING "GCC options used for every source file")

set(CMAKE_C_FLAGS_INIT "${DECOMP_GCC_FLAGS}")
# -fno-sized-deallocation: deleting destructors call operator delete(void*) like ARMCC (C++03 has no
#   sized deallocation; GCC would call operator delete(void*, unsigned) with the object size)
# -fcheck-new: the constructor only runs if new returned non-null; ARMCC tests the result of every
#   new, placement new included (e.g. ThreadPool::Setup, detail::StartAlarmThreadPool)
set(CMAKE_CXX_FLAGS_INIT "${DECOMP_GCC_FLAGS} -std=gnu++17 -fno-exceptions -fno-sized-deallocation -fcheck-new")

