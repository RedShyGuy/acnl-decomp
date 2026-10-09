# acnl-decomp

Decompilation of **Animal Crossing: New Leaf - Welcome amiibo** (3DS, USA, version 1.5). The game's code name is *garden*. The goal is C++ source that does what the original `code.bin` and CRO modules do, function by function. It is built with GCC (devkitARM); Nintendo's compiler (ARMCC 4.1) is not used, so functions are compared with the original by structure instead of byte for byte (see "Checking").

Every known class, method and data structure is declared; most methods still have an empty body that waits to be decompiled. 
For the current numbers run the `check` target and then the `progress` target (see "Building"): it lists the finished functions and bytes per library and for the game.

| | |
|---|---|
| classes (RTTI, code.bin + CROs) | ~3 600 |
| virtual methods | ~12 400 (incl. 1 265 in CROs) |
| named non-virtual methods / free functions | ~3 500 / ~800 |
| CRO modules | 38 |
| save structures | `garden_plus.dat`: complete layout, size-checked; other save file structures need to be added (friendX.dat, exhibition.dat, mailX.dat) |
| system calls (`nn::svc`) | 92 (+ the C wrapper `svcSleepThread`), the 18 wrappers ACNL contains copied 1:1 |

## Roadmap

The first step is to fully decompile any standard libraries used in ACNL.
This includes nn, nw, pead, sead, mw, libms, imgdb and cfl.
Beginning with nn:

| | |
|---|---|
| nn::ac | ✅ written |
| nn::applet | ✅ written |
| nn::boss | ✅ written |
| nn::camera | ✅ written |
| nn::cec | ✅ written |
| nn::cfg | ✅ written |
| nn::crypto | ✅ written |
| nn::dbm | ✅ written |
| nn::dsp | ✅ written |
| nn::enc | ✅ written |
| nn::err | ✅ written |
| nn::erreula | ✅ written |
| nn::fnd | ✅ written |
| nn::friends | ✅ written |
| nn::fs | ✅ written |
| nn::fslow | ✅ written |
| nn::gr | ✅ written |
| nn::gxlow | ✅ written |
| nn::hid | ✅ written |
| nn::hidlow | ✅ written |
| nn::http | ✅ written |
| nn::init | ✅ written |
| nn::ir | ✅ written |
| nn::jpeg | ✅ written |
| nn::math | ✅ written |
| nn::mic | ✅ written |
| nn::ndm | ✅ written |
| nn::nfc | ✅ written |
| nn::nfp | ✅ written |
| nn::ngc | ✅ written |
| nn::nstd | ✅ written |
| nn::nwm | ✅ written |
| nn::os | ✅ written |
| nn::pia | ✅ written |
| nn::pl | ✅ written |
| nn::ptm | ✅ written |
| nn::ro | ✅ written |
| nn::snd | ✅ written |
| nn::socket | ✅ written |
| nn::srv | ✅ written |
| nn::ssl | ✅ written |
| nn::svc | ✅ written |
| nn::ubl | ✅ written |
| nn::uds | ✅ written |
| nn::ulcd | ✅ written |
| nn::util | ✅ written |
| nn::y2r | ✅ written |
| nn::nex | ❌ next |

"Written" means every function of the package has its C++ source. A function counts as done when
`check` rates it `equivalent`; the rest is `close` or `far`, mostly because GCC and ARMCC generate
different code (unrolled loops, inlining choices, a destructor call per return path in ARMCC, ...).
The remaining differences are explained in the comments.

State of nn without nex (2026-10-09, `progress`): 4 345 functions, 4 240 of them written (98.7% of
the bytes); 2 857 `equivalent`, 791 `close`, 592 `far`. Whole project: 3.54% of the bytes done.

## Setup

You need your own dump of the game. Nothing from the game is in this repository.

1. Dump the game with GodMode9 and extract the decrypted ExeFS / RomFS (for example with GodMode9 itself or ctrtool).
2. Turn `.code` into an ELF:
   - use any code.bin-to-ELF tool (like [CTR-elf2](https://github.com/NWPlayer123/ctr-elf2)) (text at 0x00100000, rodata at 0x0083A000, data at 0x00946000);
   - if the tool kept `.code` BLZ-compressed, run [tools/analysis/blz_fix_elf.py](tools/analysis/blz_fix_elf.py) `<in.elf> <out.elf>` to decompress it;
   - save the result as `orig/0004000000086300/code.elf`.
3. Copy `romfs:/cro/*.cro` (plus `static.crs` / `static.crr`) to `orig/0004000000086300/cro/`.
4. Install:
   - Python 3.10+;
   - CMake 3.20+ and Ninja (both ship with CLion);
   - [devkitARM](https://devkitpro.org).

## Building

```sh
cmake -B build/gcc -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/devkitarm.cmake
cmake --build build/gcc
cmake --build build/gcc --target check      # compare every implemented function with the original
cmake --build build/gcc --target progress   # done functions / bytes per library / game
```

`-DDECOMP_LINK=ON` adds the `code_elf` target, which links `code.elf` at the original addresses. It is expected to fail until enough code is decompiled. Every unit (each library, the game, each CRO module) is an object library, so `check` can compare functions one by one.

Quick feedback without building: `python tools/decomp/syntax_check.py [path filter]`.

## Checking

GCC does not produce the instructions of the original compiler, so `check` ([tools/decomp/check.py](tools/decomp/check.py), [tools/decomp/fuzzy.py](tools/decomp/fuzzy.py)) compares what a compiler cannot choose:
- called functions and their order (`memset` and `__rt_memclr` and other runtime helpers count as the same);
- member accesses: offset from the object, width, signedness, load or store;
- constants, strings and referenced addresses;
- decisions and switch tables.

```sh
python tools/decomp/check.py
python tools/decomp/check.py --function "oml::framework::Process::operator new(unsigned int)"
```

Scores of 0.97 and up are `equivalent`, 0.85 and up are `close`, the rest is `far`. With `--function` it lists what is missing or extra, for example `missing load w +0x8 / extra load h +0x8` for a member read with the wrong type. A high score is strong evidence, not proof: register use and instruction order are not compared. Functions written in assembly (the system call wrappers) are also compared byte for byte (status `match`).

The GCC options in [cmake/toolchains/devkitarm.cmake](cmake/toolchains/devkitarm.cmake) keep GCC's code close to the original's (no calloc / memset folding, blocks in source order, ...); after changing them configure again with `cmake --fresh`.

## Layout

```
include/            game headers (include/<prefix>/d<Class>.h, like the original dFoo.cpp names)
  Sv/               save data: SvGardenPlus, SvPlayer, SvNpc, SvTownData, ...  (docs/save_format.md)
src/                game sources (src/<prefix>/d<Class>.cpp, src/anonymous/<original file>.cpp)
lib/<library>/      nn (CTR-SDK), nw (NintendoWare), sead, pead, cfl, imgdb, libms, mw
  include/ src/
modules/<Module>/   CRO modules (docs/cro.md); modules/_shared = classes used by several modules
config/0004000000086300/
  symbols.json      every class (RTTI, vtables) and named function with source + tier
  modules/*.json    the same per CRO module
  extracted_data.json  data blocks without source (firmware, tables): copied from your code.elf
                    when code.elf is linked, never stored in the repository
  inputs/           raw inputs of the analysis (libgarden symbols, reference matches, hand names, ...)
orig/0004000000086300/       your dump: code.elf, cro/*.cro  (ignored by git)
ghidra/             symbols, data types and scripts for Ghidra (ghidra/README.md)
tools/analysis/     binary analysis: RTTI, vtables, xrefs, CRO parsing, hand named symbols, disassembly
tools/decomp/       check, score diff, progress, syntax check, linker script, data extraction, Ghidra export
cmake/              toolchain (devkitarm) and build helpers
docs/               naming.md, cro.md, save_format.md
```

## Workflow

1. Pick a function. `python tools/decomp/progress.py --functions-csv build/functions.csv` lists them all with size and status.
2. Read the original: in Ghidra (names and types: [ghidra/README.md](ghidra/README.md)) or with `python tools/analysis/disasm.py <start> <end>` (named calls and literals); `python tools/analysis/find_refs.py <address>` finds callers and users. Write the C++ in the existing empty body.
3. Fix the return type: the stub declarations use `void` because mangled names don't contain it.
4. Build and run `check`. A function is done when it is `equivalent`; `check --function "<name>"` shows what still differs.
5. `python tools/decomp/score_diff.py` runs `check` and lists the scores that changed since its last run.

The class headers and stubs were generated once from symbols.json. They are normal source files now: new classes and functions are written by hand.

## Naming

Names only go in when they are certain (details in [docs/naming.md](docs/naming.md)). Unknown things stay `vf_0x24`, `unk_0x10`, `SvPlayerUnk8D4C`. Every declaration says where its name comes from, for example `// 0x0064DB90 | libgarden [tier A]`, `// 0x0013098C (name after 3dbrew)` or `// 0x003DADC8 (name is ours)`. Names from Nintendo's SDK (headers, sources, documentation) are never used.

## Credits

- Save research: Slattz' [ACNL_Research](https://github.com/Slattz/ACNL_Research) with additions from the [Vapecord-ACNL-Plugin](https://github.com/RedShyGuy/Vapecord-ACNL-Plugin).
- [libgarden](https://github.com/Pienco/libgarden) symbols (CC0).
- [3dbrew](https://www.3dbrew.org/wiki/Main_Page) (SVC table, IPC command names, CRO format, result codes) and [libctru](https://github.com/devkitPro/libctru) (system call register usage).
- [Reference symbols](https://www.3dbrew.org/wiki/Titles_With_Code_Symbols): Nintendogs + Cats, Fire Emblem Fates, Mario Kart 7 (Download Play).
- [ac-decomp](https://github.com/ACreTeam/ac-decomp) project structure inspiration

AI-assisted development was used during the reconstruction of parts of the project. AI-generated code is reviewed, tested against the original binary where possible, and validated against the project's build and behavioral requirements. Reverse-engineering decisions, structure definitions, constraints, and validation methodology are maintained by the project author.

## License

The project's own files are released under CC0 1.0, see [LICENSE](LICENSE).

This project is an independent reverse-engineering effort. No original Nintendo source code or SDK source files are included.
