# Compare the symbol names in this Ghidra program with ACNL symbol files
# (ghidra/symbols/code_<version>.txt, "<name> <address> <f|l>")
# and optionally take over the names from the files.
# Same behaviour as ACNLSyncSymbols.java, but runs in Ghidra's Jython (no compile step).
#
# Modes:
#   1  report only                   - nothing is changed, a CSV report is written
#   2  apply names                   - differing / missing names are set from the files;
#                                      old label names stay as secondary labels
#   3  apply names + remove old ones - like 2, and the old secondary labels at those
#                                      addresses are deleted
# Addresses that are not in the files are never touched.
#
# @category ACNL
# @author   acnl-decomp

import codecs
from ghidra.program.model.symbol import SourceType, SymbolType

MODE_REPORT = "1 - Report only (change nothing)"
MODE_APPLY = "2 - Apply the names from the file"
MODE_CLEAN = "3 - Apply + delete the old names at those addresses"


def load_file(f, entries):
    n = 0
    fp = codecs.open(f.getAbsolutePath(), "r", "utf-8")
    try:
        for line in fp:
            p = line.strip().split()
            if len(p) < 3 or not p[1].startswith("0x"):
                continue
            a = int(p[1][2:], 16)
            is_fn = p[2] == "f"
            if is_fn:
                a &= ~1                      # Thumb functions live at the even address
            if a not in entries:             # first file / first line wins
                entries[a] = (p[0], is_fn)
                n += 1
    finally:
        fp.close()
    println("%s: %d entries" % (f.getName(), n))


def split_path(full):
    """'a::b<c::d>::e' -> ['a', 'b<c::d>', 'e'] (no split inside template brackets)"""
    parts, depth, last, i = [], 0, 0, 0
    while i < len(full):
        c = full[i]
        if c == "<":
            depth += 1
        elif c == ">":
            depth -= 1
        elif depth == 0 and full.startswith("::", i):
            parts.append(full[last:i])
            last = i + 2
            i += 1
        i += 1
    parts.append(full[last:])
    return parts


def namespace_for(st, path):
    ns = currentProgram.getGlobalNamespace()
    for part in path[:-1]:
        child = st.getNamespace(part, ns)
        if child is None:
            child = st.createNameSpace(ns, part, SourceType.IMPORTED)
        ns = child
    return ns


def set_name(st, addr, name, is_fn):
    path = split_path(name)
    simple = path[-1]
    ns = namespace_for(st, path)
    f = getFunctionAt(addr)
    if is_fn and f is not None:
        f.setName(simple, SourceType.IMPORTED)
        if f.getParentNamespace() != ns:
            f.setParentNamespace(ns)
        return
    s = st.getSymbol(simple, addr, ns)
    if s is None:
        s = st.createLabel(addr, simple, ns, SourceType.IMPORTED)
    s.setPrimary()


def remove_old_labels(st, addr, keep):
    n = 0
    for s in st.getSymbols(addr):
        if (s.getSymbolType() == SymbolType.LABEL and not s.isPrimary()
                and s.getSource() != SourceType.DEFAULT and s.getName(True) != keep):
            if s.delete():
                n += 1
    return n


def main():
    entries = {}
    args = list(getScriptArgs())
    if args:
        # headless: <mode 1|2|3> <report.csv> <symbol file> [<symbol file> ...]
        from java.io import File
        mode = {"1": MODE_REPORT, "2": MODE_APPLY, "3": MODE_CLEAN}[args[0]]
        report = File(args[1])
        for f in args[2:]:
            load_file(File(f), entries)
    else:
        load_file(askFile("Choose a symbol file (e.g. code_0004000000086300.txt)", "Load"), entries)
        while askYesNo("Another file?", "Load another symbol file (e.g. one of symbols/cro)?"):
            load_file(askFile("Choose another symbol file", "Load"), entries)
        mode = askChoice("Mode", "What should happen?", [MODE_REPORT, MODE_APPLY, MODE_CLEAN], MODE_REPORT)
        report = askFile("Save the report as (CSV)", "Save")
    println("entries loaded: %d" % len(entries))

    apply_names = mode != MODE_REPORT
    clean = mode == MODE_CLEAN

    st = currentProgram.getSymbolTable()
    same = differ = missing = applied = removed = failed = 0
    rep = codecs.open(report.getAbsolutePath(), "w", "utf-8")
    try:
        rep.write(u"address;status;ghidra_name;ghidra_source;file_name;other_labels_at_address\n")
        for a in sorted(entries.keys()):
            if monitor.isCancelled():
                break
            name, is_fn = entries[a]
            addr = toAddr(a)
            prim = st.getPrimarySymbol(addr)
            cur = prim.getName(True) if prim is not None else u""
            is_default = prim is None or prim.getSource() == SourceType.DEFAULT
            if not is_default and cur == name:
                same += 1
                continue
            if is_default:
                status = "missing"
                missing += 1
            else:
                status = "differs"
                differ += 1
            others = u" ".join(s.getName(True) for s in st.getSymbols(addr)
                               if s != prim and s.getSource() != SourceType.DEFAULT)
            rep.write(u"0x%08X;%s;%s;%s;%s;%s\n" % (a, status, cur,
                      prim.getSource().toString() if prim is not None else u"", name, others))
            if not apply_names:
                continue
            try:
                set_name(st, addr, name, is_fn)
                applied += 1
                if clean and status == "differs":
                    removed += remove_old_labels(st, addr, name)
            except Exception as ex:
                failed += 1
                printerr("0x%08X %s: %s" % (a, name, ex))
    finally:
        rep.close()
    println("same: %d, different: %d, missing: %d" % (same, differ, missing))
    if apply_names:
        println("applied: %d, old labels deleted: %d, errors: %d" % (applied, removed, failed))
    println("report: " + report.getAbsolutePath())


main()
