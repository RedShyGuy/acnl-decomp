# CRO modules

Most of ACNL's scenes are not in `code.bin`. They are loaded at runtime from `romfs:/cro/*.cro` with `nn::ro`, the system's dynamic linker. USA 1.5 has 38 modules (`ModuleShop`, `ModuleMuseum`, `ModuleTour`, ...).

## Format in short

A CRO is a relocatable module with a fixed header (`CRO0`) and these parts:

- **segments:** seg0 = .text, seg1 = .rodata, seg2 = .data, seg3 = .bss. A segment+offset pair addresses everything.
- **exports:** named and indexed, used by other modules.
- **imports:** symbols from other modules or from the static module.
- **relocations:** internal ones, patched when the module is loaded.

`static.crs` describes the static module (`code.bin`) in the same format, so CROs can import from it. `static.crr` lists the SHA-256 hashes of every CRO that may be loaded (`nn::ro` checks them).

ACNL's modules import from the static module **anonymously**: there are no names, only the static module's segment + offset (seg0 = 0x00100000, seg1 = 0x0083A000, seg2 = 0x00946000). Calls into `code.bin` go through thunks:

```
ldr pc, [pc, #-4]      ; E51FF004
.word <address in code.bin>
```

The thunk word is filled in on load. [tools/analysis/crothunks.py](../tools/analysis/crothunks.py) resolves every thunk to its target, so the code.bin function a module calls is known by name.

## In this project

| what | where |
|---|---|
| original modules | `orig/USA_1_5/cro/<Module>.cro` (not distributed) |
| classes, vtables, functions, imports per module | `config/USA_1_5/modules/<Module>.json` (from [tools/analysis/crortti.py](../tools/analysis/crortti.py)) |
| headers / sources | `modules/<Module>/include`, `modules/<Module>/src` |
| classes used by several modules | `modules/_shared/` |
| Ghidra names inside each module | `ghidra/symbols/cro/<Module>.txt` (offsets relative to the file) |

A module is laid out like a library:
- Classes that only exist in one module go to that module.
- Classes that exist in several modules go to `_shared`.
- Classes that also exist in code.bin stay in the game.

Virtual methods carry their offset inside the .cro, for example `// ModuleShop.cro +0x01A2C4 slot 0x10`.

### Build

Every module is a CMake object library `module_<Module>`, compiled like the rest of the code. Turning the objects back into a `.cro` needs two more steps:
- a link with the module's import/export table
- the CRO packer

Neither is part of this project. To build `.cro` files, point `DECOMP_MAKECRO` at a script that takes:

```
--module <Module> --objects <.o files> --symbols config/USA_1_5/modules/<Module>.json
--original orig/USA_1_5/cro/<Module>.cro --output build/.../cro/<Module>.cro
```

That enables a `<Module>_cro` target per module. `check.py` compares only code.bin functions so far; module functions are not compared yet.

### Rules for module code

- A module may call any function of code.bin. Declare it normally; the thunk is the linker's job.
- code.bin never calls into a module directly, only through virtual functions or function pointers that the module registers.
- Anonymous-namespace classes keep their original file name, for example `modules/ModuleShop/src/anonymous/...`. When the file name is unknown the file is `unknown.cpp` until the name turns up.
