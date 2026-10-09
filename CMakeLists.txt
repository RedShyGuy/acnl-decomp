# Animal Crossing: New Leaf - Welcome amiibo (USA 1.5) decompilation.
#
# Configure (GCC from devkitARM; the only toolchain):
#   cmake -B build/gcc -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/devkitarm.cmake
#
# Targets:
#   all             compile every unit (libraries, game, CRO modules) to objects
#   code_elf        link code.elf (DECOMP_LINK=ON; fails until all code is decompiled)
#   check           compare the compiled functions with orig/<version>/code.elf (structure, fuzzy.py)
#   progress        print the decompilation progress (needs a previous check)
#   syntax_check    GCC syntax check of all sources (fast, no objects)
cmake_minimum_required(VERSION 3.20)

if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchains/devkitarm.cmake" CACHE FILEPATH "")
endif()

project(acnl_decomp LANGUAGES C CXX)

# compile_commands.json in the build directory: lets IDEs (VS Code IntelliSense, clangd) parse the
# sources exactly like devkitARM's GCC does (asm volatile, __attribute__((naked)), include paths)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(DECOMP_VERSION "0004000000086300" CACHE STRING "Game version to build")
set(DECOMP_CONFIG "${CMAKE_SOURCE_DIR}/config/${DECOMP_VERSION}")
set(DECOMP_ORIG "${CMAKE_SOURCE_DIR}/orig/${DECOMP_VERSION}")
option(DECOMP_LINK "Link code.elf from the compiled objects" OFF)
set(DECOMP_MAKECRO "" CACHE STRING "Command that packs a module's objects into a .cro (optional)")

if(NOT EXISTS "${DECOMP_ORIG}/code.elf")
    message(WARNING "orig/${DECOMP_VERSION}/code.elf is missing - see README.md (\"Setup\").")
endif()

find_package(Python3 REQUIRED COMPONENTS Interpreter)
include(cmake/Decomp.cmake)

decomp_global_includes()
add_compile_definitions(DECOMP_VERSION_${DECOMP_VERSION}=1)

# --------------------------------------------------------------- units
# libraries (lib/<name>): CTR-SDK (nn), NintendoWare (nw), sead / pead, ...
file(GLOB lib_dirs LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/lib/*")
foreach(dir ${lib_dirs})
    if(IS_DIRECTORY "${dir}/src")
        get_filename_component(name "${dir}" NAME)
        decomp_add_unit(lib_${name} "${dir}/src" LIBRARY)
    endif()
endforeach()

# the game itself (static part of code.bin)
decomp_add_unit(garden "${CMAKE_SOURCE_DIR}/src" GAME)

# CRO modules (romfs:/cro/*.cro), modules/_shared holds classes used by several modules
file(GLOB module_dirs LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/modules/*")
foreach(dir ${module_dirs})
    if(IS_DIRECTORY "${dir}/src")
        get_filename_component(name "${dir}" NAME)
        decomp_add_module(${name})
    endif()
endforeach()

get_property(units GLOBAL PROPERTY DECOMP_UNITS)

# --------------------------------------------------------------- link
if(DECOMP_LINK)
    set(ldscript "${CMAKE_BINARY_DIR}/code.ld")
    add_custom_command(OUTPUT "${ldscript}"
        COMMAND Python3::Interpreter "${CMAKE_SOURCE_DIR}/tools/decomp/gen_ldscript.py"
                --version ${DECOMP_VERSION} -o "${ldscript}"
        DEPENDS "${CMAKE_SOURCE_DIR}/tools/decomp/gen_ldscript.py" "${DECOMP_CONFIG}/symbols.json"
        COMMENT "Generating linker script")
    add_custom_target(ldscript DEPENDS "${ldscript}")

    # data blobs of the original that are not source (config/<version>/extracted_data.json):
    # copied from the user's orig/<version>/code.elf into the build directory
    set(extracted_dir "${CMAKE_BINARY_DIR}/extracted")
    add_custom_command(OUTPUT "${extracted_dir}/extracted_data.c"
        COMMAND Python3::Interpreter "${CMAKE_SOURCE_DIR}/tools/decomp/extract_data.py"
                --version ${DECOMP_VERSION} --output-dir "${extracted_dir}"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        DEPENDS "${CMAKE_SOURCE_DIR}/tools/decomp/extract_data.py" "${DECOMP_CONFIG}/extracted_data.json"
                "${DECOMP_ORIG}/code.elf"
        COMMENT "Extracting data blobs from the original code.elf")
    add_library(extracted_data OBJECT "${extracted_dir}/extracted_data.c")

    set(static_units ${units})
    list(FILTER static_units EXCLUDE REGEX "^module_")
    set(objects "$<TARGET_OBJECTS:extracted_data>")
    foreach(u ${static_units})
        list(APPEND objects "$<TARGET_OBJECTS:${u}>")
    endforeach()
    add_executable(code_elf ${objects})
    add_dependencies(code_elf ldscript)
    set_target_properties(code_elf PROPERTIES OUTPUT_NAME code SUFFIX ".elf" LINKER_LANGUAGE CXX)
    target_link_options(code_elf PRIVATE -nostartfiles -nostdlib -T ${ldscript} -Wl,--no-gc-sections -Wl,-e,0x00100000)
endif()

# --------------------------------------------------------------- tools
add_custom_target(check
    # every extern data declaration must be listed in extracted_data.json (or code.elf lacks it)
    COMMAND Python3::Interpreter "${CMAKE_SOURCE_DIR}/tools/decomp/extract_data.py"
            --version ${DECOMP_VERSION} --check
    COMMAND Python3::Interpreter "${CMAKE_SOURCE_DIR}/tools/decomp/check.py"
            --version ${DECOMP_VERSION} --build-dir "${CMAKE_BINARY_DIR}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    DEPENDS ${units}
    USES_TERMINAL
    COMMENT "Comparing compiled functions with the original")

add_custom_target(progress
    COMMAND Python3::Interpreter "${CMAKE_SOURCE_DIR}/tools/decomp/progress.py"
            --version ${DECOMP_VERSION} --build-dir "${CMAKE_BINARY_DIR}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    USES_TERMINAL)

add_custom_target(syntax_check
    COMMAND Python3::Interpreter "${CMAKE_SOURCE_DIR}/tools/decomp/syntax_check.py"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    USES_TERMINAL)
