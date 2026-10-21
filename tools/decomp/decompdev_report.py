#!/usr/bin/env python3
"""Generate/validate a decomp.dev progress report (objdiff report v2) from ACNL's check results.

decomp.dev reads a report.json that a GitHub Actions run on the default branch uploads as an
artifact named "<version>_report". objdiff cannot compare our objects (GCC vs. the ARMCC original),
so the report is built from check.py's results instead: a function counts as matched when
check.py rates it "equivalent" (or byte "match"); its fuzzy percent is the structural score.

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
LIBRARIES = ("nn", "nw", "sead", "pead", "imgdb", "mw", "libms", "cfl")
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
    "runtime": "Runtime / C++ library",
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
    top = namespace_of(name)
    if not top or top.startswith("FUN_") or top.startswith("sub_"):
        top = "unnamed"
    return f"unassigned/{top}"


def measures(functions: list[dict[str, Any]], units: int = 0) -> dict[str, Any]:
    total_code = sum(f["_size"] for f in functions)
    matched_code = sum(f["_size"] for f in functions if f["_complete"])
    total_functions = len(functions)
    matched_functions = sum(1 for f in functions if f["_complete"])
    weighted = sum(f["fuzzy_match_percent"] * f["_size"] for f in functions)
    return {
        "fuzzy_match_percent": round(weighted / total_code, 6) if total_code else 0.0,
        "total_code": json_u64(total_code),
        "matched_code": json_u64(matched_code),
        "matched_code_percent": round(100.0 * matched_code / total_code, 6) if total_code else 0.0,
        "total_data": "0",
        "matched_data": "0",
        "matched_data_percent": 0.0,
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


def build_report(build_dir: Path, version: str) -> dict[str, Any]:
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

    report_units = []
    by_category: dict[str, list[dict[str, Any]]] = defaultdict(list)
    for unit_name in sorted(units):
        functions = units[unit_name]
        # a unit belongs to the category of most of its code
        weights: dict[str, int] = defaultdict(int)
        for fn in functions:
            weights[fn["_category"]] += fn["_size"]
        category = max(sorted(weights), key=lambda c: weights[c])
        by_category[category].extend(functions)
        metadata: dict[str, Any] = {"progress_categories": [category], "auto_generated": unit_name.startswith("unassigned/")}
        if not unit_name.startswith("unassigned/"):
            metadata["source_path"] = unit_name
        report_units.append({
            "name": unit_name,
            "measures": measures(functions, 1),
            "functions": [{k: v for k, v in fn.items() if not k.startswith("_")} for fn in functions],
            "metadata": metadata,
        })

    all_functions = [fn for functions in units.values() for fn in functions]
    categories = [
        {"id": c, "name": CATEGORY_NAMES.get(c, c), "measures": measures(by_category[c], sum(1 for u in report_units if u["metadata"]["progress_categories"] == [c]))}
        for c in sorted(by_category)
    ]
    return {
        "measures": measures(all_functions, len(report_units)),
        "units": report_units,
        "version": REPORT_VERSION,
        "categories": categories,
    }


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
    print(f"Fuzzy: {m['fuzzy_match_percent']:.3f}%; matched code: {m['matched_code_percent']:.3f}%")
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
