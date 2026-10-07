# Ghidra

Everything needed to get a fully named Ghidra project of ACNL USA 1.5.

| path | contents |
|---|---|
| `symbols/code_0004000000086300.txt` | function and data names for code.elf (`<name> <address> <f\|l>`): tier A/B names of `config/0004000000086300/symbols.json` (including the hand-named ones), the names of every decompiled function and global from our sources, typeinfo/vtable labels |
| `symbols/cro/<Module>.txt` | names inside each CRO (offset relative to the .cro file) |
| `types/acnl_save.h` | the save structures (`SvGardenPlus`, `SvPlayer`, ...) as plain C |
| `scripts/ACNLSyncSymbols.py` | compares / applies a symbol file (Jython) |
| `scripts/ACNLSyncSymbols.java` | the same for Ghidra versions that can compile Java scripts |
| `scripts/ACNLSvcNames.py` | names every `svc` instruction (equate + comment), optionally renames `nn::svc` wrappers |
| `create_project.py` | creates a project headless: import + analysis + both scripts |

Regenerate the files after a build:

```
python tools/decomp/check.py                                         # names of the decompiled code
python tools/analysis/manual_names.py orig/0004000000086300/code.elf build/analysis   # hand-named globals
python tools/decomp/export_ghidra.py                                 # --analysis-dir build/analysis
```

`build/analysis/ghidra_symbols.txt` (typeinfo/vtable labels) comes from `tools/analysis/analyze.py`;
it only changes with the binary. Never pass `--json` to analyze.py: `symbols.json` is maintained by hand.

## New project (headless)

```
python ghidra/create_project.py --ghidra <Ghidra installation directory>
```

This creates `build/ghidra/ACNL_0004000000086300.gpr`. Auto analysis of code.elf takes a while.

## Existing project

1. Script Manager → add `ghidra/scripts` to the script directories.
2. Run `ACNLSvcNames.py`.
3. Run `ACNLSyncSymbols.py`:
   - choose `symbols/code_0004000000086300.txt`;
   - mode 1 only writes a CSV report;
   - mode 2 applies names and keeps old labels;
   - mode 3 applies names and deletes the old labels at those addresses.
4. File → Parse C Source…:
   - add `types/acnl_save.h` (it needs no extra parse options or include paths);
   - press *Parse to Program*.

   The types appear under `acnl_save.h` in the Data Type Manager.

## Notes

- Beware that not every name is at the proper offset! Slowly all of them get checked and properly seated.
- Names never contain parameters (Ghidra keeps the signature separately).
- Thumb functions are listed at their even address.
- If Ghidra cannot compile the `.java` script (this happens when the JDK it runs on does not fit the Ghidra release), use the `.py` version.
- For a CRO, convert it first: `python tools/analysis/cro2elf.py <out dir> orig/0004000000086300/cro/*.cro`. The `.elf` is relocated (vtables, pointers, import thunks to code.bin are filled in), has real sections and carries the names from `symbols/cro/*.txt`. Address = `0x40000000` + CRO file offset.
- Without conversion: import the `.cro` file as raw binary at base 0 (ARM:LE:32:v6). The offsets in `symbols/cro/*.txt` are file offsets, so `ACNLSyncSymbols.py` can apply them directly. Relocations are then missing (vtables are 0).
