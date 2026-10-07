// Compare the symbol names in this Ghidra program with ACNL symbol files
// (ghidra/symbols/code_<version>.txt, format: "<name> <address> <f|l>")
// and optionally take over the names from the files.
//
// Modes:
//   1  Report only                   - nothing is changed, a CSV report is written
//   2  Apply names                   - differing / missing names are set from the files;
//                                      old label names stay as secondary labels
//   3  Apply names + remove old ones - like 2, and the old secondary labels at those
//                                      addresses are deleted
//
// Addresses that do not appear in the files are never touched, so names you gave
// yourself elsewhere stay as they are. Function names that get replaced are listed
// in the report, so nothing is lost without a trace.
//
// @category ACNL
// @author   acnl-decomp

import java.io.*;
import java.util.*;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.*;

public class ACNLSyncSymbols extends GhidraScript {

    private static final String MODE_REPORT = "1 - Report only (change nothing)";
    private static final String MODE_APPLY = "2 - Apply the names from the file";
    private static final String MODE_CLEAN = "3 - Apply + delete the old names at those addresses";

    private static class Entry {
        String name;
        boolean isFunction;
        Entry(String name, boolean isFunction) { this.name = name; this.isFunction = isFunction; }
    }

    @Override
    protected void run() throws Exception {
        Map<Long, Entry> entries = new LinkedHashMap<>();
        File first = askFile("Choose a symbol file (e.g. code_0004000000086300.txt)", "Load");
        loadFile(first, entries);
        while (askYesNo("Another file?", "Load another symbol file (e.g. one of symbols/cro)?")) {
            loadFile(askFile("Choose another symbol file", "Load"), entries);
        }
        println("entries loaded: " + entries.size());

        String mode = askChoice("Mode", "What should happen?",
                Arrays.asList(MODE_REPORT, MODE_APPLY, MODE_CLEAN), MODE_REPORT);
        boolean apply = !mode.equals(MODE_REPORT);
        boolean clean = mode.equals(MODE_CLEAN);

        File reportFile = askFile("Save the report as (CSV)", "Save");
        SymbolTable st = currentProgram.getSymbolTable();

        int same = 0, differ = 0, missing = 0, applied = 0, removed = 0, failed = 0;
        try (PrintWriter rep = new PrintWriter(new OutputStreamWriter(new FileOutputStream(reportFile), "UTF-8"))) {
            rep.println("address;status;ghidra_name;ghidra_source;file_name;other_labels_at_address");
            for (Map.Entry<Long, Entry> me : entries.entrySet()) {
                if (monitor.isCancelled()) {
                    break;
                }
                Address addr = toAddr(me.getKey());
                Entry e = me.getValue();
                Symbol prim = st.getPrimarySymbol(addr);
                String cur = prim == null ? "" : prim.getName(true);
                boolean isDefault = prim == null || prim.getSource() == SourceType.DEFAULT;
                String status;
                if (!isDefault && cur.equals(e.name)) {
                    same++;
                    continue;                       // identical: not listed in the report
                } else if (isDefault) {
                    status = "missing";
                    missing++;
                } else {
                    status = "differs";
                    differ++;
                }
                StringBuilder others = new StringBuilder();
                for (Symbol s : st.getSymbols(addr)) {
                    if (s != prim && s.getSource() != SourceType.DEFAULT) {
                        others.append(s.getName(true)).append(' ');
                    }
                }
                rep.println(String.format("0x%08X;%s;%s;%s;%s;%s", me.getKey(), status, cur,
                        prim == null ? "" : prim.getSource().toString(), e.name, others.toString().trim()));

                if (!apply) {
                    continue;
                }
                try {
                    setName(st, addr, e);
                    applied++;
                    if (clean && status.equals("differs")) {
                        removed += removeOldLabels(st, addr, e.name);
                    }
                } catch (Exception ex) {
                    failed++;
                    printerr(String.format("0x%08X %s: %s", me.getKey(), e.name, ex.getMessage()));
                }
            }
        }
        println("same: " + same + ", different: " + differ + ", missing: " + missing);
        if (apply) {
            println("applied: " + applied + ", old labels deleted: " + removed + ", errors: " + failed);
        }
        println("report: " + reportFile.getAbsolutePath());
    }

    private void loadFile(File f, Map<Long, Entry> entries) throws IOException {
        int n = 0;
        try (BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(f), "UTF-8"))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] p = line.trim().split("\\s+");
                if (p.length < 3 || !p[1].startsWith("0x")) {
                    continue;
                }
                long a = Long.parseLong(p[1].substring(2), 16);
                boolean fn = p[2].equals("f");
                if (fn) {
                    a &= ~1L;                       // Thumb functions live at the even address
                }
                // the first file / first line wins if an address appears twice
                if (!entries.containsKey(a)) {
                    entries.put(a, new Entry(p[0], fn));
                    n++;
                }
            }
        }
        println(f.getName() + ": " + n + " entries");
    }

    /** split "a::b<c::d>::e" into [a, b<c::d>, e] (no split inside template brackets) */
    private static List<String> splitPath(String full) {
        List<String> parts = new ArrayList<>();
        int depth = 0, last = 0;
        for (int i = 0; i < full.length(); i++) {
            char c = full.charAt(i);
            if (c == '<') depth++;
            else if (c == '>') depth--;
            else if (depth == 0 && c == ':' && i + 1 < full.length() && full.charAt(i + 1) == ':') {
                parts.add(full.substring(last, i));
                last = i + 2;
                i++;
            }
        }
        parts.add(full.substring(last));
        return parts;
    }

    private Namespace namespaceFor(SymbolTable st, List<String> path) throws Exception {
        Namespace ns = currentProgram.getGlobalNamespace();
        for (int i = 0; i < path.size() - 1; i++) {
            Namespace child = st.getNamespace(path.get(i), ns);
            if (child == null) {
                child = st.createNameSpace(ns, path.get(i), SourceType.IMPORTED);
            }
            ns = child;
        }
        return ns;
    }

    private void setName(SymbolTable st, Address addr, Entry e) throws Exception {
        List<String> path = splitPath(e.name);
        String simple = path.get(path.size() - 1);
        Namespace ns = namespaceFor(st, path);
        Function f = getFunctionAt(addr);
        if (e.isFunction && f != null) {
            f.setName(simple, SourceType.IMPORTED);
            if (!f.getParentNamespace().equals(ns)) {
                f.setParentNamespace(ns);
            }
            return;
        }
        Symbol s = st.getSymbol(simple, addr, ns);
        if (s == null) {
            s = st.createLabel(addr, simple, ns, SourceType.IMPORTED);
        }
        s.setPrimary();
    }

    private int removeOldLabels(SymbolTable st, Address addr, String keep) {
        int n = 0;
        for (Symbol s : st.getSymbols(addr)) {
            if (s.getSymbolType() == SymbolType.LABEL && !s.isPrimary()
                    && s.getSource() != SourceType.DEFAULT && !s.getName(true).equals(keep)) {
                if (s.delete()) {
                    n++;
                }
            }
        }
        return n;
    }
}
