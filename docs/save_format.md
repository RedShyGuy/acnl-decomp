# Save data (`garden_plus.dat`)

The save file of Welcome amiibo is `garden_plus.dat` (`garden` is the game's code name). Its layout is in [include/Sv/](../include/Sv/). The headers are normal source files: edit them directly. The research they build on is credited in the [README](../README.md#credits).

## Layout

| file offset | type | header | contents |
|---|---|---|---|
| `0x00000` | `SvSecureValueHeader` | dSvGardenPlus.h | secure value, "initialised" flag |
| `0x00080` | `SvSaveHeader` | dSvGardenPlus.h | header checksum, save version |
| `0x000A0` | `SvPlayer[4]` | dSvPlayer.h | the 4 players, 0xA480 bytes each |
| `0x292A0` | `SvNpcData` | dSvNpc.h | the 10 villagers + move-in candidates |
| `0x4BE80` | `SvStrcData` | dSvStrc.h | buildings, public works, design stands |
| `0x5033C` | `SvMinigameData` | dSvGardenPlus.h | minigames (Welcome amiibo) |
| `0x52C30` | `SvGardenDataUnk52BB0` | dSvGardenPlus.h | unknown (Welcome amiibo) |
| `0x53424` | `SvTownData` | dSvTown.h | map, houses, shops, museum, island |
| `0x71900` | `SvSharedData` | dSvShared.h | census, mailboxes, secret storage |
| `0x89B00` | | | end of file |

Every structure has an `ASSERT_SIZE`, so a wrong offset stops the build.

## Conventions

- **Members** use lowerCamelCase.
- **Unknown members** are named `unk_0x<offset>` and **padding** is `pad_0x<offset>`. Offsets are relative to the struct. Bits get `_<bit>`, for example `unk_0x12_3`. If the research gave an unknown member a hint name, it is kept as a comment (`research name: ...`).
- **Unknown structures** are named `<Parent>Unk<offset>`, for example `SvPlayerUnk8D4C`.
- **Enumerations** follow the game's own style (`SvFgName::Name`, `PlayerAction::Name`): `struct SvInitiative { enum Name { ... }; };`. Members of such a type are stored as `u8` / `u16` / `u32` with a comment naming the enum. This keeps the layout independent of how a compiler sizes enums.
- **Packing:** the structures are `#pragma pack(1)` because several members sit at offsets the natural alignment doesn't allow, for example 64 bit values at 2-aligned offsets. The original types there are probably arrays or small structs. Replace them when the code shows what they really are.

## Ghidra

[tools/decomp/export_ghidra.py](../tools/decomp/export_ghidra.py) flattens these headers into plain C ([ghidra/types/acnl_save.h](../ghidra/types/acnl_save.h)) for Ghidra's *Parse C Source*. Enumerators are prefixed with their enum name there (`SvGulliverMail_Japan`) because C has a single scope for them.
