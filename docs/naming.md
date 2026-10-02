# Names and how certain they are

**Rule:** a name goes into the project only when it is certain. A wrong name is worse than `vf_0x24` or `unk_0x10`, because everyone builds on it.

## Where names come from

| source | example | used for |
|---|---|---|
| RTTI of code.bin and the CROs | `AcPlayer`, `nn::nex::RootTransport` | class names and hierarchy, vtables |
| libgarden symbols (USA 1.5) | `SvFgName::GetFgKind() const` | methods of game classes |
| reference games with symbols: Nintendogs + Cats (memmap), Fire Emblem Fates (StackTrace files), Mario Kart 7 Download Play (xmap) | `sead::Heap::alloc`, `nn::svc::CreateThread` | sead, nn, nw library functions |
| strings in the binary | `dBsShowMgr.cpp`, `seadTextWriter.cpp`, `BsSvMgr::stepLoad` | original file names, method names in asserts |
| IPC command headers + [3dbrew](https://www.3dbrew.org/wiki/Services) command names | `nn::y2r::CTR::detail::Y2r::SetRotation` | IPC wrappers (`manual:3dbrew`); the class is the one of the same IPC session that other sources confirm |
| hand analysis | `HeapInit::CreateDLLHeap` | marked `manual:*` |

## Tiers (config/USA_1_5/symbols.json)

| tier | meaning | in the sources |
|---|---|---|
| A | confirmed: several sources agree, or exact byte match plus a consistent call graph | declared |
| B | one reliable source with a byte match | declared |
| C | only the call structure matches | only when the decompiled code confirms it |
| X | sources contradict each other | not declared |
| `manual:ref-verified` | verified by hand against reference binaries | declared |
| `manual:3dbrew` | IPC command name from 3dbrew, class from the IPC session | declared |
| `manual:ours`, `manual:heapinit` | descriptive names, not original symbols | declared, comment says so |
| `manual:inferred` | reasoned by hand, not proven | declared, comment says so |

Every declaration carries its source and tier, for example `// 0x0064DB90 | libgarden [tier A]`.

## Placeholders

- `vf_0xNN`: virtual method at vtable offset 0xNN, name unknown. The **introducing** class (the first class with that slot) owns the name. Overrides use the same name.
- `struct X { u32 _unknown; }; // placeholder`: a type that only appears in signatures. When the real type is known, write it in its own header and replace the placeholder in `include/forward.h` with a forward declaration that names the header (as done for `nn::Handle`, `nn::Result`).
- `unk_0x<offset>` / `pad_0x<offset>`: unknown members / padding (save structures).
- `void` return types: mangled names do not contain the return type, so the stubs declare `void`. Fix it when you decompile the function.

## File names

| code | header | source |
|---|---|---|
| game class `Foo` | `include/<prefix>/dFoo.h` | `src/<prefix>/dFoo.cpp` |
| game class in a namespace `ns::Foo` | `include/ns/dFoo.h` | `src/ns/dFoo.cpp` |
| sead | `lib/sead/include/sead/<dir>/seadFoo.h` | `lib/sead/src/...` |
| nn / nw | `lib/nn/include/nn/<module>/<module>_Foo.h` | `lib/nn/src/...` |
| free functions of a namespace | `<ns>_Api.h` / `<ns>_Api.cpp` | |
| anonymous-namespace classes | | `src/anonymous/<original file>.cpp` |
| CRO module class | `modules/<Module>/include/...` | `modules/<Module>/src/...` |

The `d` prefix, `sead` prefix and `<module>_` prefix copy the original file names, which survive as strings in the binary (`dBsShowMgr.cpp`, `seadTextWriter.cpp`, `fs_Api.cpp`, `applet_API.cpp`).

`<prefix>` is the class name's prefix (`Ac`, `Bs`, `Sv`, `Npc`, ...). Classes without one go to `Other/`.
