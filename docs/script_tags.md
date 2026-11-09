# Script tags (MSBT control codes) -> naming proposal

A tag in a message text is `0x000E, group, index, size, args`. The handlers are named `TagN_M` so far (TalkSeq, TalkMeasure,
ChoiceSeq, MailSeq, UiSeq, MailMeasure, InsertEngine, InsertSearcher …), the words they insert are the `GetTagN_M`/`UpdateTagN_M`
slots of `script::Insert` and the `GetTagN_M` slots of `script::IRecept`.

Sources: libgarden `script/Tag.hpp` (L), Slattz ACNL_MSBT_Tags.txt (S), the handler code (C), example texts of the English
romfs (E), the message files that use the tag (F), the source of the value (V: the `Update` slot of Insert, or the game classes
whose recept implements the `IRecept` slot, found through their vtables). Confidence: **sure** (source or unambiguous code),
**likely** (examples and files agree), **open** (to be decided). Entries marked **→ for you** need your decision. Decision 2026-11-08: only the **sure** names are used in the code; likely and open ones stay `TagN_M` until the game code that fills them is decompiled.

How the values are found (2026-11-08): most words are filled by `Insert::UpdateX` — from a string list (`STR_Week`, `STR_Common` …),
the date, the save data, or a slot of the current recept. The recepts are game classes that are not decompiled yet; their
implementations are known by address (V), but what they read only becomes clear once they are decompiled. Words that no recept
implements (4/18, 17/4, 17/5, 17/8, 20/x …) are written by game code straight into the Insert; their callers are game code too.

## Group 0 — text format (L: PREDEFINED)

| tag | proposed name | evidence | confidence |
|---|---|---|---|
| 0/0 | RUBY | L; C: main text + ruby | sure |
| 0/1 | FONT | L (handler empty) | sure |
| 0/2 | SCALE | L; C: scale in percent | sure |
| 0/3 | SET_COLOR | L, S (colors 0..6, 0xFFFF default) | sure |
| 0/4 | PAGE_BREAK | S "text break"; C: state 2 (page end) | sure |

## Group 4 — numbers, date and time

| tag | proposed name | evidence | confidence |
|---|---|---|---|
| 4/0 | YEAR | V year of the date of the talk; E "Ringing in [4/0]" | sure |
| 4/1 | MONTH_NAME | V `STR_Month`; E "birthday's in [4/1]" | sure |
| 4/2 | DAY | V `STR_Day`; E "On [4/1] [4/2], [4/0]" | sure |
| 4/3 | HOUR | V hour (12/24 h by region); E "It's already [4/3]:[4/4]" | sure |
| 4/4 | MINUTE | V; E as above | sure |
| 4/5 | SECOND | V | sure |
| 4/6 … 4/12 | NUMBER_1 … NUMBER_7 | C: number of the recept with 1…7 digits (`UpdateTag4_6(index, digits)`); E spaces left (1), month/day (2), points (3), Bells (4–5), HHA points (7) | sure |
| 4/13 | RECEPT_YEAR | V year of index of the recept; E "[4/14] [4/15], [4/13]" | sure |
| 4/14 | RECEPT_MONTH | V `GetMonth(index)` → `STR_Month` | sure |
| 4/15 | RECEPT_DAY | V `GetDay(index)` → `STR_Day` | sure |
| 4/16 | APPOINTMENT_HOUR | E "Today from [4/16] p.m.", "See you at [4/16]:[4/17] [5/24]"; F BBS_Kodanuki (10), *_Q08_Reserve / *_Q08_Con (villager appointments); V set by game code (no recept) | likely — **→ for you:** "appointment" fits the villager files, but the shop notice (BBS_Kodanuki, sale "from [4/16] p.m.") uses it too; maybe neutral GIVEN_HOUR? |
| 4/17 | APPOINTMENT_MINUTE | as 4/16 | likely |
| 4/18 | ? (Bells amount) | E "Your current balance is [4/18]", "a total of [4/18] Bells at closing"; F only NPC_Tanukichi_SellVillage (6) and NPC_Obj_Common (1); V no recept implements it, game code writes it | open — **→ for you:** which Tom Nook conversation is "SellVillage", and is the amount a balance or a payout? |
| 4/19 | FISH_SIZE | F SYS_Get_Fish (75), SYS_Get_Diving (30), NPC_UomasaA/B (fishing tourney); V AcPlayer / PlayerSimpleMessage recept 0x2FBE34; Insert formats it as a decimal number by region | likely |
| 4/20 | INSECT_SIZE | E "[4/20]mm"; F SYS_Get_Insect (75); V AcPlayer recept 0x2FBF6C, decimal by region | likely |

## Group 5 — words

| tag | proposed name | evidence | confidence |
|---|---|---|---|
| 5/0 | ? | S "current player name"; F talk 1489, letters 46 (To 9); V talk: Func56DE5C(4) (a game function, purpose of the number unknown), letters: recept slot | open (user 2026-11-08: not yet) |
| 5/1 | ? (the one who speaks / writes) | F talk 1319, letters 828 (From); V talk: the name of the recept (`ITalkRecept::m_Name`), letters: the sender (recept slot) | open (user 2026-11-08: not a letter-only name; decide later) |
| 5/2, 5/3 | ? (another villager) | E "the other day [5/2] told me", "And then [5/3] passed by"; F villager rumor / request talks; V villager recept 0x63A3C4 / 0x63A2F8 (call 0x7145C0 / 0x56E35C, not decompiled) | open — **→ for you:** two different "other villager" words; resident vs. former resident? |
| 5/4 | ? (villager by index) | E "The other day with [5/4], we"; F *_FreeA_Always, *_ApA_Always; V Word228 (index), villager recept 0x63E7E0 (random pick, SearchCand<10>) | open |
| 5/5 | ? (town) | V the town data of the save (`townData.townData1`); F talk 1679, Bbs 27 | open (user 2026-11-08) |
| 5/6 | ? (town) | V the town ID in the personal data of the current player (`playerInfo.townData`); S "Isabelle startup"; equal to 5/5 in the own save, different only for a player from another town (assumption from the code) | open (user 2026-11-08) |
| 5/7 | CATCHPHRASE | E "[14/3][5/7]!" at line ends; F every villager file; V villager recept 0x635400 | likely — **→ for you:** confirm |
| 5/8 | PLAYER_NICKNAME | E "Welcome, [5/8]"; F villager files; V Word238, villager recept 0x637738 (checks the current player) | likely — **→ for you:** the name/nickname the villager uses for the player? |
| 5/9 | WEEKDAY | V `STR_Week` of the date | sure |
| 5/10 | IMPRESSION | V `STR_ImpressF/M` | sure |
| 5/11 | CONSTELLATION | V `STR_Constellation` (zodiac sign of the player) | sure |
| 5/12 | ? | E "Hey, did you know I'm [5/12]?", "the [5/12] horoscope"; F *_Ev_Aprilfool_Hint, *_Ev_NpcbdayH; V villager recept 0x643C60 (NPC table 0xA84AEC, SvFgName) | open — **→ for you:** a property of the villager in birthday / April Fools hints |
| 5/13 | GENDER_TEXT | C InsertGenderText | sure |
| 5/14 | DRINK | V `STR_Drink` | sure |
| 5/15 | OBJECT | V `STR_ObjectF/M` | sure |
| 5/16 | SWEETS | V `STR_Sweets` | sure |
| 5/17 | SPORTS | V `STR_Sports` | sure |
| 5/18 | HOBBY | V `STR_HobbyF/M` | sure |
| 5/19 | FOOD | V `STR_Food` | sure |
| 5/20 | MUSIC | V `STR_Music` | sure |
| 5/21 | RANDOM_SONG | V a random item of the 91 IDs from 0x212B (SvFgName); E "K.K. Slider's [5/21]", "listen to [5/21]" | likely — **→ for you:** are 0x212B–0x2185 the K.K. songs in your item ID list? |
| 5/22 | AM_PM | V `STR_Common` 20/21 by the hour (regions with a 12-hour clock) | sure |
| 5/23 | PART_OF_DAY | V `STR_Common` 15–19 by time::GetPartOfDay (Japan uses it instead of 5/22) | sure |
| 5/24 | APPOINTMENT_AM_PM | V `STR_Common` am/pm of 4/16 | sure |
| 5/25 | APPOINTMENT_PART_OF_DAY | V `STR_Common` by part of day of 4/16 (Japan instead of 5/24) | sure |
| 5/26 | TOWN_FRUIT | V the fruit of the town | sure |
| 5/27 | MAYOR_NAME_2 | V the player Func71AD8C picks (as 5/28, only another null test); E "before [5/27] became the new mayor" | likely |
| 5/28 | MAYOR_NAME | V as 5/27; E "Mayor [5/28]" | likely |
| 5/29 | BOOK | V `STR_Book` | sure |
| 5/30 | SEASON | V `STR_Common` by time::GetSeason | sure |
| 5/31 | ? (index) | E "Is that a wet [5/31]?!"; F *_Furniture, *_Ev_Harvest, *_Ev_Xmas; V villager recept 0x63567C (NPC table 0xA84AEC) | open — **→ for you:** a word of the villager ("favorite thing"?) |
| 5/32 | ? | F NPC_Hakkemie (4), NPC_Obj_Fortune_Tel (2); V player recept 0x2FB2A0 (a string of StrMgr by an index from 0x5C116C) | open — **→ for you:** the fortune teller's word (lucky item / fortune?) |
| 5/33 | RANDOM_FLOWER? | V a random item of item set 74 (SvFgName); E "a fine bouquet of [5/33]"; F *_3P_Zk | likely — **→ for you:** is set 74 the flowers? |
| 5/34 | ? (villager) | E "near [5/34]'s house", "[5/34] said you did"; F ZK_Re_Worry (6); V Word298 (index), villager recept 0x63A0DC | open |
| 5/35 | FASHION_LOOK | E "the [5/35] look"; F *_FreeA_ClothesN, NPC_Kate; V villager recept 0x6413BC | likely |
| 5/36 | ? (a player) | V Func56DE5C(0) (5/0 is Func56DE5C(4) = the current player); E "hang out with [5/36]"; F *_FreeG_Visitor, NPC_Cleaning_Visit, *_AiV_* (visits) | open — **→ for you:** the visiting / other player? |
| 5/37 | ? | V game code only; F SYS_Error (1) | open |

## Group 7 — text control

| tag | proposed name | evidence | confidence |
|---|---|---|---|
| 7/0 | PAUSE | S "pause frames"; C | sure |
| 7/1 | WAIT_BUTTON | S "wait for A/B"; C | sure |
| 7/2 / 7/3 | INSTANT_BEGIN / INSTANT_END | C (text without typing) | sure |
| 7/4 / 7/5 | NO_FAST_BEGIN / NO_FAST_END | C (fast forward locked) | sure |
| 7/6 / 7/7 | WORD_RANGE_BEGIN / WORD_RANGE_END | C (InsertWord2B0Range) | sure |
| 7/8 | AUTO_NEXT | S "auto next"; C (time) | sure |
| 7/9 | AUTO_NEXT_ONLY | C (time, button disabled) | sure |
| 7/10 … 7/12 | SHAKE_0 … SHAKE_2 | C (window shake, parameters 0..2) | sure |
| 7/13 … 7/17 | RECEPT_7_13 … RECEPT_7_17 | C (ITalkRecept::OnTag7) | sure (technical) |
| 7/18 | MINI_GAME | E rock-paper-scissors, "Made Ya Look" | likely |

## Groups 8 / 15 — choices

| tag | proposed name | evidence | confidence |
|---|---|---|---|
| 15/0 … 15/4 | CHOICE_2 … CHOICE_6 | S "choice without cancel"; C SetChoice15(n) | sure |
| 15/5 … 15/9 | CHOICE_2_CANCEL … CHOICE_6_CANCEL | S "with cancel"; C | sure |
| 15/10 | CHOICE_LIST | C (ReadChoices, fixed) | sure |
| 8/0 … 8/4 | CHOICE_JUMP_2 … CHOICE_JUMP_6 | C SetChoice8 (labels to jump to) | sure |
| 8/5 … 8/9 | CHOICE_JUMP_2_CANCEL … | C | sure |
| 8/10 | CHOICE_JUMP_LIST | C | sure |

## Group 10 — branches (C: Branch(label of the condition))

Named after the condition in the code: 10/0, 10/11, 10/23, 10/28, 10/30 BRANCH_RECEPT_n; 10/3, 10/4 BRANCH_PERCENT; 10/6 BRANCH_HOUR;
10/7 BRANCH_PART_OF_DAY; 10/9 BRANCH_WEEKDAY; 10/16 BRANCH_SEASON; 10/17 BRANCH_MONTH; 10/21 BRANCH_OUTDOOR; 10/14, 10/15
BRANCH_NET_PLAY…; the others after their functions (several call game functions that are not decompiled yet → open).

## Group 13 — sound and effects

13/0 SOUND, 13/1 ISLAND_GAME_MUSIC, 13/2 UPA_CLOSE_MUSIC, 13/3/13/4/13/5 RECEPT_13_n, 13/6/13/7 SONG_NOTE_END/SONG_NOTE,
13/8 CHOICE_KIND, 13/9/13/10 FX_BEGIN/FX_END, 13/11/13/12 SPEECH_FILTER_BEGIN/END — from the code, sure.

## Group 14 — grammar

14/3 and 14/4 only set flags of the insert frame that change how the next word is written (F: 14/3 before most names in villager
talk, 14/4 in titles "～ [14/4][5/5] News ～"); maybe "capitalize" vs. "title case" — open.

14/0 WORD_INFO, 14/1 ARTICLE_2, 14/2 ARTICLE_1, 14/3 ?, 14/4 ?, 14/5 GENDER_P, 14/6 SEX_TEXT, 14/7 GENDER_N, 14/8 GENDER,
14/9 NUMBER_FORM, 14/10 ELISION, 14/11…14/13 NUMBER_TEXT_4…6, 14/14/14/15 PATCHIM_FORM, 14/16/14/17 EPENTHESIS_A/E,
14/18 GENDER_EX — the debug strings of the binary (ELISION, GENDER, GENDER_N, GENDER_P, GENDER_EX) and the code.

## Groups 17 / 18 — names of things

| tag | files (F) | value (V) | proposal |
|---|---|---|---|
| 17/0 | *_Re_FirstD (28 each), SYS_2D_UI (39); E "HAPPY BIRTHDAY Mayor [17/0]!" | Word2B4 (index), recept NoticeWork::ReceptFoot | PLAYER_NAME_BY_INDEX? — **→ for you** |
| 17/1 | SYS_Door (42), *_Q06_A_Report, *_FreeA_Friend; E "Today is [17/1]'s birthday" | Word2B8 (index); S "owner name" | OWNER_NAME (S) — likely |
| 17/2 | *_Re_FirstD, SYS_2D_UI; E "Town: [17/2]", "The [17/2] Museum" | Word2BC (index) | TOWN_NAME_BY_INDEX? — **→ for you** |
| 17/3 | NPC_Honma_Commentary, *_Q02_Give, NPC_Frankrin, *_Furniture | Word2C0 (index); recepts AcPlayer, AcFtr*, BsMenuItem | ITEM_NAME — likely |
| 17/4 | *_FreeA_Memory | GetTag17_4(index), game code only | ? — **→ for you** (memories of the villager) |
| 17/5 | NPC_Kinuyo (10), *_FreeA_ClothesN; E "erase [17/0]'s [17/5]?" | game code only | DESIGN_NAME? — **→ for you** |
| 17/6 | *_FreeB_Obj; E "help us complete the [17/6]" | Word2CC (index), game code only | PUBLIC_WORKS_PROJECT — likely |
| 17/7 | NPC_Dream, BBS_Default; E "This town's Dream Address: [17/7]" | GetTag0_4 formatted as hex | DREAM_ADDRESS — sure |
| 17/8 | SYS_2D_UI (1) | game code only | ? |
| 17/9 | NPC_Kate, NPC_Kinuyo; E "to look like [17/9]" | Word2D8 (index) | ? — **→ for you** |
| 18/0 … 18/34 | e.g. 18/1 *_Q12_Sign ("Get signatures for the [18/1] issue"), 18/2 *_Ev_Xmas_Hint (present), 18/20 *_FreeA_Rumor (wish), 18/21 NPC_Tourguide_Common (tour), 18/31 SYS_Get_Badge (badge) | Word2AC(kind, index) / GetTag5_32(n): the recept of the villager / player gives a word by kind | named per tag after its file / example — **→ for you:** confirm per tag |

## Groups without a handler in the script code

1 (scrapped), 2 (speaking styles of special characters), 3 (villager emotions, S), 9, 11, 12 (expressions), 16, 19 (dialog
values, DialogCompo) — handled by the recepts or not at all; names only where a source exists.
