"""The data of code.bin for the decomp.dev report: which bytes of .rodata/.data/.bss our sources define.

Counted as defined (an item of the unit, i.e. the object, that defines it):
- global variables with an address comment in the sources (check.source_variables), sized by our
  object's symbol; initialized ones are compared with the original bytes (words our object
  relocates, i.e. pointers, count as equal), .bss ones match when the original is .bss as well,
- the vtables (all copies ARMCC made), typeinfo objects and typeinfo names of the classes of
  symbols.json that our objects emit; their slots are checked through the functions,
- the blobs of config/<version>/extracted_data.json (copied from the original when linking).

Everything else (string literals, compiler-made tables, data of code we did not write yet) is not
defined; decompdev_report.py puts it into one auto-generated unit per section.
"""
from __future__ import annotations

import glob
import json
import os
import re
import struct
import subprocess
import sys
from collections import defaultdict
from pathlib import Path
from typing import Any

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(HERE.parent / "analysis"))
import check  # noqa: E402
import elf32  # noqa: E402

# the data sections of code.elf: name -> (start, end, has bytes)
DATA_SECTIONS = (".rodata", ".data", ".bss")


class Original:
    """The data sections of the original code.elf."""

    def __init__(self, path: Path):
        e = elf32.Elf(str(path))
        self.sections: dict[str, tuple[int, int]] = {}
        self.bytes: dict[str, bytes] = {}
        for s in e.sections:
            if s.name in DATA_SECTIONS:
                self.sections[s.name] = (s.addr, s.addr + s.size)
                self.bytes[s.name] = s.data if s.type != 8 else b""

    def section_of(self, address: int) -> str | None:
        for name, (lo, hi) in self.sections.items():
            if lo <= address < hi:
                return name
        return None

    def read(self, address: int, size: int) -> bytes | None:
        """the bytes at the address (None in .bss)"""
        name = self.section_of(address)
        if name is None or name == ".bss":
            return None
        lo = self.sections[name][0]
        return self.bytes[name][address - lo:address - lo + size]

    def u32(self, address: int) -> int:
        data = self.read(address, 4)
        return struct.unpack("<I", data)[0] if data and len(data) == 4 else 0

    def cstr_size(self, address: int) -> int:
        name = self.section_of(address)
        if name is None or name == ".bss":
            return 0
        lo = self.sections[name][0]
        data = self.bytes[name]
        end = data.find(b"\0", address - lo)
        return end - (address - lo) + 1 if end >= 0 else 0


def unit_of_object(path: str) -> str | None:
    p = path.replace("\\", "/")
    m = re.match(r"(?:.*/)?CMakeFiles/([^/]+)\.dir/(.+?)\.(?:obj|o)$", p)
    if not m or m.group(1).startswith("module_"):
        return None
    return m.group(2)


def typeinfo_size(cls: dict[str, Any]) -> int:
    kind = cls.get("kind")
    if kind == "si":
        return 12
    if kind == "vmi":
        return 16 + 8 * len(cls.get("bases", []))
    return 8


def vtable_span(cls: dict[str, Any]) -> list[tuple[int, int]]:
    """(address, size) of the vtables of a class"""
    out = []
    for vt in cls.get("vtables", []):
        out.append((int(vt["addr"]), int(vt["vptr"]) - int(vt["addr"]) + 4 * len(vt.get("entries", []))))
    return out


def word_equality(ours: bytes, theirs: bytes, relocated: set[int]) -> float:
    """the share of equal words (relocated words count as equal)"""
    words = max(1, (len(ours) + 3) // 4)
    equal = 0
    for i in range(0, len(ours), 4):
        if i in relocated or ours[i:i + 4] == theirs[i:i + 4]:
            equal += 1
    return equal / words


def collect(build_dir: Path, version: str) -> tuple[Original, list[dict[str, Any]]]:
    """the original's data sections and the defined data items:
    {unit, name, address, size, section, percent}"""
    root = HERE.parent.parent
    original = Original(root / "orig" / version / "code.elf")
    symbols = json.loads((root / "config" / version / "symbols.json").read_text(encoding="utf-8"))
    variables = check.source_variables()
    by_bare: dict[str, list[str]] = defaultdict(list)
    for key in variables:
        by_bare[re.split(r"::", key)[-1]].append(key)
    classes = {check.norm(c["name"]): c for c in symbols.get("classes", [])}

    objects = sorted(glob.glob(str(build_dir / "CMakeFiles" / "**" / "*.obj"), recursive=True)
                     + glob.glob(str(build_dir / "CMakeFiles" / "**" / "*.o"), recursive=True))
    found: list[tuple[str, elf32.Elf, elf32.Symbol]] = []
    for path in objects:
        unit = unit_of_object(os.path.relpath(path, build_dir))
        if unit is None:
            continue
        try:
            e = elf32.Elf(path)
        except (OSError, ValueError):
            continue
        for s in e.symbols:
            if s.type == elf32.STT_OBJECT and s.size > 0 and 0 < s.shndx < len(e.sections):
                found.append((unit, e, s))
    demangled = check.demangle(sorted({s.name for _, _, s in found}))

    items: dict[int, dict[str, Any]] = {}

    def add(unit: str, name: str, address: int, size: int, percent: float) -> None:
        section = original.section_of(address)
        if section is None or size <= 0 or address in items:
            return
        items[address] = {"unit": unit, "name": name, "address": address, "size": size,
                          "section": section, "percent": percent}

    used_keys: set[str] = set()
    for unit, e, s in found:
        dn = demangled.get(s.name, s.name)
        key = check.norm(dn)
        sec = e.sections[s.shndx]
        # vtables and typeinfo of the classes of symbols.json
        m = re.match(r"(vtable|typeinfo name|typeinfo) for (.+)$", dn)
        if m:
            cls = classes.get(check.norm(m.group(2)))
            if cls is None:
                continue
            if m.group(1) == "vtable":
                # every copy ARMCC made (one per translation unit that needed it)
                for address, size in vtable_span(cls):
                    add(unit, dn, address, size, 100.0)
            elif m.group(1) == "typeinfo":
                add(unit, dn, int(cls["typeinfo"]), typeinfo_size(cls), 100.0)
            else:
                address = original.u32(int(cls["typeinfo"]) + 4)
                add(unit, dn, address, original.cstr_size(address), 100.0)
            continue
        # the variables of the sources; a function's static by its name alone
        address = variables.get(key)
        if address is None:
            bare = re.split(r"::", key)[-1]
            candidates = [k for k in by_bare.get(bare, []) if k not in used_keys]
            if len(candidates) == 1 and "::" in dn and dn.split("(")[0] != dn:
                key = candidates[0]
                address = variables[key]
        if address is None:
            continue
        used_keys.add(key)
        theirs = original.read(address, s.size)
        if sec.type == 8:                       # ours in .bss: equal if the original is zero
            ok = theirs is None or not any(theirs)
            percent = 100.0 if ok else 0.0
        elif theirs is None:                    # ours initialized, the original in .bss
            percent = 100.0 if not any(sec.data[s.value:s.value + s.size]) else 0.0
        else:
            relocated = {off - s.value for off, _, _ in e.relocations(sec) if s.value <= off < s.value + s.size}
            percent = 100.0 * word_equality(sec.data[s.value:s.value + s.size], theirs, relocated)
        add(unit, dn, address, s.size, round(percent, 3))

    # the blobs copied from the original
    extracted = root / "config" / version / "extracted_data.json"
    if extracted.exists():
        for blob in json.loads(extracted.read_text(encoding="utf-8")).get("blobs", []):
            address = int(str(blob.get("address", blob.get("addr", 0))), 0)
            size = int(str(blob.get("size", 0)), 0)
            add(blob.get("unit") or "extracted_data", blob.get("name", "blob"), address, size, 100.0)

    # an item ends where the next one starts (our size may be larger than the original's)
    ordered = sorted(items.values(), key=lambda it: it["address"])
    for cur, nxt in zip(ordered, ordered[1:] + [None]):
        end = original.sections[cur["section"]][1]
        if nxt is not None and nxt["section"] == cur["section"]:
            end = min(end, nxt["address"])
        cur["size"] = max(0, min(cur["size"], end - cur["address"]))
    return original, [it for it in ordered if it["size"] > 0]
