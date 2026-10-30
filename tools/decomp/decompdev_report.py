#!/usr/bin/env python3
"""Generate/validate a decomp.dev progress report (objdiff report v2) from ACNL's check results.

decomp.dev reads a report.json that a GitHub Actions run on the default branch uploads as an
artifact named "<version>_report". objdiff cannot compare our objects (GCC vs. the ARMCC original),
so the report is built from check.py's results instead: a function counts as matched when
check.py rates it "equivalent" (or byte "match"); its fuzzy percent is the structural score.

Data: the .rodata/.data/.bss bytes of code.bin that our sources define (variables with an address
comment, compared byte by byte; vtables and RTTI of our classes; extracted blobs) count per unit
(report_data.py). The rest of each section is in auto-generated "unassigned/data/..." units, by
category where the defined data around a gap belongs to one category.

Modules: every CRO of config/<version>/modules is a unit "modules/<Module>" (module_name/module_id,
category "modules") with its functions and data segments. check.py does not compare module code
yet, so it counts as 0%.

The game dump is not on GitHub, so CI cannot build or check. Generate the report locally after a
full build and a check.py run, and commit the snapshot:

  python tools/decomp/check.py                                   (writes build/gcc/check/results.json)
  python tools/decomp/decompdev_report.py --build-dir build/gcc   (writes report/ snapshot)

CI only validates the snapshot (--check-snapshot) and uploads it.
"""
from __future__ import annotations

import argparse
import gzip
import hashlib
import json
import re
import subprocess
from collections import defaultdict
from pathlib import Path
from typing import Any

ROOT = Path(__file__).resolve().parents[2]
VERSION_DEFAULT = "0004000000086300"
REPORT_DIR = ROOT / "report"
REPORT_GZ_PATH = REPORT_DIR / "report.json.gz"
METADATA_PATH = REPORT_DIR / "report-metadata.json"
REPORT_VERSION = 2
TEXT_BASE = 0x00100000

# libraries with their own directory under lib/ (and their top-level namespaces)
LIBRARIES = ("nn", "nw", "sead", "pead", "imgdb", "mw", "libms", "cfl", "clib")
# the ARM C library of the game (lib/clib): compiler helpers, C functions and the C++ ABI helpers of the
# ARM runtime
CLIB_FUNCTION = re.compile(
    r'(__aeabi_|__rt_|_ll_|__ARM_|__hardfp_|__softfp_|__fpl_|__mathlib_|__btod_|__support_|__memmove_|_memcpy_|__cxa_|__I\$)'
    r'|(_memset|_e2d|_e2e|_frem|bsearch|wcslen|mbsinit|mbrlen|mbrtowc|memcmp|memcpy|memset|strncat|tolower|towupper'
    r'|snprintf|sprintf|vsprintf|vsnprintf|swprintf|vswprintf|strtoull|wcstoul|wcstombs|wcscpy|wcsncpy|wmemcpy'
    r'|wmemmove|wcscat|wcscmp|wcsncmp|wcsstr|wcsicmp|strcat|strncmp|strlcpy|strncpy|strcmp|strlen|strcpy'
    r'|abort|memmoveWithResult|memcpyWithResult|memcpyWithResultEntry|__read_errno|__set_errno|wcstok|wmemset|memchr|towlower|__mbrtowc_byte|__cpp_initialize__aeabi_|__printf_\w+|_udiv10|__wcrtomb_char|__isdigit|ferror|printf|__vfprintf|_btod_etento|__fp_status|fputc|__flsbuf|__digit_value|__strtoul_core|__wcstoul_core|__strtoull_core|strtol|atoi|strtoul|__strtoull|__wctype_classes|iswspace|llabs|__scanf_\w+|sscanf|__stdio_\w+|__qsort_partitions|qsort|strchr|strstr|strrchr|strcoll|ftell|__fseek|wcrtomb)$')
# runtime and C++ library namespaces that have no source directory of their own
RUNTIME_NAMESPACES = ("std", "__rw", "__cxxabiv1")
CATEGORY_NAMES = {
    "game": "Game",
    "nn": "CTR-SDK (nn)",
    "nw": "NintendoWare (nw)",
    "sead": "sead",
    "pead": "pead",
    "imgdb": "imgdb",
    "mw": "mw",
    "libms": "libms",
    "cfl": "cfl",
    "clib": "ARM C library (clib)",
    "runtime": "Runtime / C++ library",
    "codebin": "code.bin (all)",
    "modules": "CRO modules",
}
# inputs of the report; the fingerprint covers the git-tracked (and new, not ignored) files
FINGERPRINT_DIRS = ("src", "lib", "modules", "include", "cmake", "tools/decomp", "tools/analysis")
FINGERPRINT_SUFFIXES = {".cpp", ".c", ".h", ".hpp", ".inc", ".s", ".py", ".cmake", ".txt", ".json", ".tsv"}


def json_u64(value: int) -> str:
    # objdiff's protobuf JSON mapping writes uint64 values as decimal strings
    return str(max(0, int(value)))


def git_files(paths: list[str]) -> list[str] | None:
    """The tracked and untracked (not ignored) files under the paths, or None without git."""
    try:
        out = subprocess.run(
            ["git", "-C", str(ROOT), "ls-files", "--cached", "--others", "--exclude-standard", "-z", "--", *paths],
            check=True, capture_output=True,
        ).stdout
    except (OSError, subprocess.CalledProcessError):
        return None
    return sorted({p for p in out.decode("utf-8").split("\0") if p})


def source_fingerprint(version: str) -> str:
    """Hash of the inputs of the report (line endings normalized: CI checks out LF, Windows may have CRLF)."""
    candidates = list(FINGERPRINT_DIRS) + ["CMakeLists.txt", f"config/{version}"]
    files = git_files(candidates)
    if files is None:
        files = []
        for rel in candidates:
            base = ROOT / rel
            if base.is_file():
                files.append(rel)
            elif base.is_dir():
                files.extend(p.relative_to(ROOT).as_posix() for p in base.rglob("*") if p.is_file())
    h = hashlib.sha256()
    for rel in sorted(set(files)):
        path = ROOT / rel
        if not path.is_file() or (path.suffix.lower() not in FINGERPRINT_SUFFIXES and path.name != "CMakeLists.txt"):
            continue
        data = path.read_bytes().replace(b"\r\n", b"\n")
        name = rel.encode("utf-8")
        h.update(len(name).to_bytes(4, "big"))
        h.update(name)
        h.update(len(data).to_bytes(8, "big"))
        h.update(data)
    return h.hexdigest()


def load_json(path: Path) -> Any:
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except FileNotFoundError:
        raise SystemExit(f"Missing required file: {path}")
    except json.JSONDecodeError as exc:
        raise SystemExit(f"Invalid JSON in {path}: {exc}")


def namespace_of(name: str) -> str:
    return name.replace("(anonymous namespace)::", "").split("::", 1)[0].split("(", 1)[0].strip()


def category_of(unit: str, name: str) -> str:
    """The progress category of a function: by its source directory, else by its namespace."""
    m = re.match(r"lib/([^/]+)/", unit)
    if m and m.group(1) in LIBRARIES:
        return m.group(1)
    if unit.startswith("src/"):
        return "game"
    # the C functions of nn (nngxInitialize, nncxZlib_deflate, nninitSetup, nndbgPanic, ...)
    if re.match(r"nn(gx|gxlow|cx|init|dbg|nstd|os)[A-Z_]|nexZlib_|gl[A-Z]|__(sta|fb|fbv|vb|vbv|shm|shv|tx|txv|vldtr|sys|cb|mem|srf)_", name):
        return "nn"
    if CLIB_FUNCTION.match(name):
        return "clib"
    top = namespace_of(name)
    if top in LIBRARIES:
        return top
    if top in RUNTIME_NAMESPACES:
        return "runtime"
    return "game"


def unit_of(result: dict[str, Any] | None, name: str) -> str:
    """The unit (source file) of a function; functions without one are grouped by namespace."""
    if result:
        obj = str(result.get("object", "")).replace("\\", "/")
        m = re.match(r"(?:.*/)?CMakeFiles/[^/]+\.dir/(.+?)\.obj$", obj) or re.match(r"(?:.*/)?CMakeFiles/[^/]+\.dir/(.+?)\.o$", obj)
        if m:
            return m.group(1)
    if CLIB_FUNCTION.match(name):
        return "unassigned/clib"
    top = namespace_of(name)
    if not top or top.startswith("FUN_") or top.startswith("sub_"):
        top = "unnamed"
    return f"unassigned/{top}"


def measures(functions: list[dict[str, Any]], data: list[dict[str, Any]] | None = None, units: int = 0) -> dict[str, Any]:
    """objdiff measures of functions and data items ({_size, _complete}); fuzzy is weighted by code"""
    data = data or []
    total_code = sum(f["_size"] for f in functions)
    matched_code = sum(f["_size"] for f in functions if f["_complete"])
    total_functions = len(functions)
    matched_functions = sum(1 for f in functions if f["_complete"])
    weighted = sum(f["fuzzy_match_percent"] * f["_size"] for f in functions)
    total_data = sum(d["_size"] for d in data)
    matched_data = sum(d["_size"] for d in data if d["_complete"])
    return {
        "fuzzy_match_percent": round(weighted / total_code, 6) if total_code else 0.0,
        "total_code": json_u64(total_code),
        "matched_code": json_u64(matched_code),
        "matched_code_percent": round(100.0 * matched_code / total_code, 6) if total_code else 0.0,
        "total_data": json_u64(total_data),
        "matched_data": json_u64(matched_data),
        "matched_data_percent": round(100.0 * matched_data / total_data, 6) if total_data else 0.0,
        "total_functions": total_functions,
        "matched_functions": matched_functions,
        "matched_functions_percent": round(100.0 * matched_functions / total_functions, 6) if total_functions else 0.0,
        "complete_code": "0",
        "complete_code_percent": 0.0,
        "complete_data": "0",
        "complete_data_percent": 0.0,
        "total_units": units,
        "complete_units": 0,
    }


def text_item(functions: list[dict[str, Any]]) -> list[dict[str, Any]]:
    """the .text section item of a unit"""
    size = sum(f["_size"] for f in functions)
    if not size:
        return []
    weighted = sum(f["fuzzy_match_percent"] * f["_size"] for f in functions)
    return [{"name": ".text", "size": json_u64(size), "fuzzy_match_percent": round(weighted / size, 3)}]


def section_items(data: list[dict[str, Any]]) -> list[dict[str, Any]]:
    """objdiff section items of a unit: per data section its bytes and their match"""
    out = []
    for section in sorted({d["_section"] for d in data}):
        items = [d for d in data if d["_section"] == section]
        size = sum(d["_size"] for d in items)
        weighted = sum(d["_percent"] * d["_size"] for d in items)
        out.append({"name": section, "size": json_u64(size), "fuzzy_match_percent": round(weighted / size, 3) if size else 0.0})
    return out


def code_functions(build_dir: Path, version: str) -> dict[str, list[dict[str, Any]]]:
    """the functions of code.bin per unit, graded by check.py"""
    config_dir = ROOT / "config" / version
    symbols = load_json(config_dir / "symbols.json")
    results = load_json(build_dir / "check" / "results.json").get("functions", {})

    starts = sorted({int(x) & ~1 for x in symbols.get("function_starts", [])})
    if not starts:
        raise SystemExit(f"No function_starts in {config_dir / 'symbols.json'}")
    names: dict[int, str] = {}
    for function in symbols.get("functions", []):
        try:
            names.setdefault(int(function["addr"]) & ~1, str(function.get("name") or ""))
        except (KeyError, TypeError, ValueError):
            continue

    units: dict[str, list[dict[str, Any]]] = defaultdict(list)
    for index, address in enumerate(starts):
        end = starts[index + 1] if index + 1 < len(starts) else address + 4
        size = max(4, end - address)
        result = results.get(f"0x{address:08X}")
        name = (result or {}).get("name") or names.get(address) or f"FUN_{address:08X}"
        status = (result or {}).get("status")
        grade = (result or {}).get("grade")
        score = (result or {}).get("score")
        if status == "match" or grade == "equivalent":
            percent, complete = 100.0, True
        elif isinstance(score, (int, float)):
            percent, complete = max(0.0, min(100.0, float(score) * 100.0)), False
        else:
            percent, complete = 0.0, False
        unit = unit_of(result, name)
        units[unit].append({
            "name": (result or {}).get("mangled") or name,
            "size": json_u64(size),
            "fuzzy_match_percent": round(percent, 3),
            "metadata": {"demangled_name": name, "virtual_address": json_u64(address)},
            "address": json_u64(address - TEXT_BASE),
            "_size": size,
            "_complete": complete,
            "_category": category_of(unit, name),
        })
    return units


def code_data(build_dir: Path, version: str) -> tuple[dict[str, list[dict[str, Any]]], list[dict[str, Any]]]:
    """the defined data of code.bin per unit, and the rest of each data section (report_data.py)"""
    import report_data  # needs the original code.elf and the build; not imported by --check-snapshot

    original, items = report_data.collect(build_dir, version)
    units: dict[str, list[dict[str, Any]]] = defaultdict(list)
    for it in items:
        it["category"] = category_of(it["unit"], it["name"])
        units[it["unit"]].append({
            "_size": it["size"], "_complete": it["percent"] >= 100.0, "_percent": it["percent"],
            "_section": it["section"], "_category": it["category"],
        })
    # The rest of each section. ARMCC places the data of a library together, so a gap between two
    # items of the same category belongs to that category; the other gaps stay without one.
    rest: dict[tuple[str, str], int] = defaultdict(int)
    for section, (lo, hi) in original.sections.items():
        inside = [it for it in items if it["section"] == section]
        position, previous = lo, None
        for it in inside + [None]:
            start = it["address"] if it is not None else hi
            if start > position:
                same = previous is not None and it is not None and previous["category"] == it["category"]
                rest[(section, previous["category"] if same else "")] += start - position
            if it is not None:
                position, previous = max(position, it["address"] + it["size"]), it
    data_rest = [{"_size": size, "_complete": False, "_percent": 0.0, "_section": section, "_category": category}
                 for (section, category), size in sorted(rest.items())]
    return units, data_rest


def module_units(version: str) -> list[dict[str, Any]]:
    """one unit per CRO module (config/<version>/modules/<Module>.json): its functions and data
    segments. check.py does not compare module code yet, so nothing of it counts as matched."""
    out = []
    paths = sorted((ROOT / "config" / version / "modules").glob("*.json"))
    for module_id, path in enumerate(paths, start=1):
        info = load_json(path)
        module = info.get("module") or path.stem
        segments = {int(s["id"]): s for s in info.get("segments", []) if int(s.get("size", 0)) > 0}
        text = segments.get(0)
        functions: list[dict[str, Any]] = []
        if text is not None:
            lo, hi = int(text["offset"]), int(text["offset"]) + int(text["size"])
            named: dict[int, str] = {}
            for fn in info.get("functions", []):
                named.setdefault(int(fn["offset"]) & ~1, str(fn.get("name") or ""))
            starts = sorted(a for a in named if lo <= a < hi)
            if not starts or starts[0] != lo:
                starts.insert(0, lo)
            for index, start in enumerate(starts):
                end = starts[index + 1] if index + 1 < len(starts) else hi
                name = named.get(start) or f"FUN_{module}_{start:06X}"
                functions.append({
                    "name": name,
                    "size": json_u64(end - start),
                    "fuzzy_match_percent": 0.0,
                    "metadata": {"demangled_name": name},
                    "address": json_u64(start - lo),
                    "_size": end - start,
                    "_complete": False,
                })
        data = []
        for seg_id, section in ((1, ".rodata"), (2, ".data"), (3, ".bss")):
            seg = segments.get(seg_id)
            if seg is not None:
                data.append({"_size": int(seg["size"]), "_complete": False, "_percent": 0.0, "_section": section})
        out.append({
            "name": f"modules/{module}",
            "measures": measures(functions, data, 1),
            "sections": text_item(functions) + section_items(data),
            "functions": [{k: v for k, v in fn.items() if not k.startswith("_")} for fn in functions],
            "metadata": {"module_name": module, "module_id": module_id, "progress_categories": ["modules"],
                         "auto_generated": True},
            "_functions": functions,
            "_data": data,
        })
    return out


def build_report(build_dir: Path, version: str) -> dict[str, Any]:
    functions_by_unit = code_functions(build_dir, version)
    data_by_unit, data_rest = code_data(build_dir, version)

    report_units = []
    for unit_name in sorted(set(functions_by_unit) | set(data_by_unit)):
        functions = functions_by_unit.get(unit_name, [])
        data = data_by_unit.get(unit_name, [])
        # a unit belongs to the category of most of its code (of its data without code)
        weights: dict[str, int] = defaultdict(int)
        for item in functions or data:
            weights[item["_category"]] += item["_size"]
        category = max(sorted(weights), key=lambda c: weights[c])
        auto = unit_name.startswith("unassigned/")
        metadata: dict[str, Any] = {"progress_categories": [category, "codebin"], "auto_generated": auto}
        if not auto and unit_name != "extracted_data":
            metadata["source_path"] = unit_name
        report_units.append({
            "name": unit_name,
            "measures": measures(functions, data, 1),
            "sections": text_item(functions) + section_items(data),
            "functions": [{k: v for k, v in fn.items() if not k.startswith("_")} for fn in functions],
            "metadata": metadata,
            "_functions": functions,
            "_data": data,
        })
    # the data of code.bin that no source defines yet, per category and section
    for item in data_rest:
        category = item["_category"]
        report_units.append({
            "name": f"unassigned/data/{category or 'unknown'}/{item['_section']}",
            "measures": measures([], [item], 1),
            "sections": section_items([item]),
            "functions": [],
            "metadata": {"progress_categories": ([category] if category else []) + ["codebin"], "auto_generated": True},
            "_functions": [],
            "_data": [item],
        })
    report_units.extend(module_units(version))

    def total(units: list[dict[str, Any]]) -> dict[str, Any]:
        return measures([f for u in units for f in u["_functions"]], [d for u in units for d in u["_data"]], len(units))

    category_ids = sorted({c for u in report_units for c in u["metadata"]["progress_categories"]})
    categories = [
        {"id": c, "name": CATEGORY_NAMES.get(c, c),
         "measures": total([u for u in report_units if c in u["metadata"]["progress_categories"]])}
        for c in category_ids
    ]
    report = {
        "measures": total(report_units),
        "units": [{k: v for k, v in u.items() if not k.startswith("_")} for u in report_units],
        "version": REPORT_VERSION,
        "categories": categories,
    }
    return report


def generate(build_dir: Path, version: str) -> None:
    report = build_report(build_dir, version)
    data = (json.dumps(report, separators=(",", ":"), sort_keys=False) + "\n").encode("utf-8")
    REPORT_DIR.mkdir(exist_ok=True)
    # gzip with a fixed mtime so equal reports give equal files
    REPORT_GZ_PATH.write_bytes(gzip.compress(data, compresslevel=9, mtime=0))
    metadata = {
        "report_version": REPORT_VERSION,
        "game_version": version,
        "source_fingerprint": source_fingerprint(version),
        "report_sha256": hashlib.sha256(data).hexdigest(),
        "note": "Fuzzy scores are ACNL's compiler-independent structural similarity scores (check.py), mapped to "
                "objdiff's fuzzy_match_percent. Only grade=equivalent or byte status=match counts as matched.",
    }
    METADATA_PATH.write_text(json.dumps(metadata, indent=2) + "\n", encoding="utf-8")
    m = report["measures"]
    print(f"Wrote {REPORT_GZ_PATH.relative_to(ROOT)} ({len(data)} bytes JSON) and {METADATA_PATH.relative_to(ROOT)}")
    print(f"Units: {m['total_units']}; functions: {m['total_functions']}; matched/equivalent: {m['matched_functions']}")
    print(f"Fuzzy: {m['fuzzy_match_percent']:.3f}%; matched code: {m['matched_code_percent']:.3f}%; "
          f"matched data: {m['matched_data_percent']:.3f}% of {int(m['total_data'])} bytes")
    for c in report["categories"]:
        cm = c["measures"]
        print(f"  {c['name']}: code {cm['matched_code_percent']:.2f}% (fuzzy {cm['fuzzy_match_percent']:.2f}%), "
              f"data {cm['matched_data_percent']:.2f}% of {int(cm['total_data'])} bytes, {cm['total_units']} units")
    print("Commit the report/ directory together with the sources it was generated from.")


def validate(version: str, output: Path | None) -> None:
    metadata = load_json(METADATA_PATH)
    if metadata.get("report_version") != REPORT_VERSION or metadata.get("game_version") != version:
        raise SystemExit("Report metadata version mismatch; regenerate the report locally.")
    if metadata.get("source_fingerprint") != source_fingerprint(version):
        raise SystemExit(
            "The report is stale: sources/config changed since it was generated.\n"
            "Run check.py locally, regenerate the report and commit the report/ directory."
        )
    data = gzip.decompress(REPORT_GZ_PATH.read_bytes())
    if hashlib.sha256(data).hexdigest() != metadata.get("report_sha256"):
        raise SystemExit("report.json.gz does not match report-metadata.json; regenerate and commit both.")
    report = json.loads(data)
    if report.get("version") != REPORT_VERSION or not isinstance(report.get("units"), list) or not isinstance(report.get("measures"), dict):
        raise SystemExit("The report is not an objdiff v2 report.")
    if output is not None:
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_bytes(data)
        print(f"Wrote {output}")
    print(f"Report snapshot valid: {len(report['units'])} units; {report['measures'].get('total_functions', 0)} functions.")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--build-dir", default=str(ROOT / "build" / "gcc"), help="CMake build directory (default: build/gcc)")
    parser.add_argument("--version", default=VERSION_DEFAULT, help="game config/version directory")
    parser.add_argument("--check-snapshot", action="store_true", help="validate the committed report without building")
    parser.add_argument("--output", help="with --check-snapshot: write the uncompressed report.json here (for the upload)")
    args = parser.parse_args()
    if args.check_snapshot:
        validate(args.version, Path(args.output) if args.output else None)
    else:
        generate(Path(args.build_dir).resolve(), args.version)


if __name__ == "__main__":
    main()
