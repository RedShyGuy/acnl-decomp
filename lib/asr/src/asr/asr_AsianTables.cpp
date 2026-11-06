#include "asr/asr_Language.h"

// The tables of the Asian languages: katakana to romaji, romaji to phones, pinyin to phones and the sound change rules
// (a text is replaced between a left and a right context). All are data of the library.
namespace asr {
// 0x008C6500
extern const char* const s_KanaOne[] = {
    "K1\t",
    "\xA5\xA2\ta",
    "\xA5\xA4\ti",
    "\xA5\xA6\tu",
    "\xA5\xA8\te",
    "\xA5\xAA\to",
    "\xA5\xAB\tk a",
    "\xA5\xAD\tk i",
    "\xA5\xAF\tk u",
    "\xA5\xB1\tk e",
    "\xA5\xB3\tk o",
    "\xA5\xB5\ts a",
    "\xA5\xB7\tsh i",
    "\xA5\xB9\ts u",
    "\xA5\xBB\ts e",
    "\xA5\xBD\ts o",
    "\xA5\xBF\tt a",
    "\xA5\xC1\tch i",
    "\xA5\xC4\tts u",
    "\xA5\xC6\tt e",
    "\xA5\xC8\tt o",
    "\xA5\xCA\tn a",
    "\xA5\xCB\tn i",
    "\xA5\xCC\tn u",
    "\xA5\xCD\tn e",
    "\xA5\xCE\tn o",
    "\xA5\xCF\th a",
    "\xA5\xD2\th i",
    "\xA5\xD5\tf u",
    "\xA5\xD8\th e",
    "\xA5\xDB\th o",
    "\xA5\xDE\tm a",
    "\xA5\xDF\tm i",
    "\xA5\xE0\tm u",
    "\xA5\xE1\tm e",
    "\xA5\xE2\tm o",
    "\xA5\xE4\ty a",
    "\xA5\xE6\ty u",
    "\xA5\xE8\ty o",
    "\xA5\xE9\tr a",
    "\xA5\xEA\tr i",
    "\xA5\xEB\tr u",
    "\xA5\xEC\tr e",
    "\xA5\xED\tr o",
    "\xA5\xEF\tw a",
    "\xA5\xAC\tg a",
    "\xA5\xAE\tg i",
    "\xA5\xB0\tg u",
    "\xA5\xB2\tg e",
    "\xA5\xB4\tg o",
    "\xA5\xB6\tz a",
    "\xA5\xB8\tj i",
    "\xA5\xBA\tz u",
    "\xA5\xBC\tz e",
    "\xA5\xBE\tz o",
    "\xA5\xC0\td a",
    "\xA5\xC2\tj i",
    "\xA5\xC5\tz u",
    "\xA5\xC7\td e",
    "\xA5\xC9\td o",
    "\xA5\xD0\tb a",
    "\xA5\xD3\tb i",
    "\xA5\xD6\tb u",
    "\xA5\xD9\tb e",
    "\xA5\xDC\tb o",
    "\xA5\xD1\tp a",
    "\xA5\xD4\tp i",
    "\xA5\xD7\tp u",
    "\xA5\xDA\tp e",
    "\xA5\xDD\tp o",
    "\xA5\xF4\tv u",
    "\xA5\xF2\to",
    "\xA5\xF3\tN",
    "\xA1\xBC\t-",
    "\xA5\xC3\tq",
    "\xA5\xA1\ta",
    "\xA5\xA3\ti",
    "\xA5\xA5\tu",
    "\xA5\xA7\te",
    "\xA5\xA9\to",
    "\xA5\xE3\ty a",
    "\xA5\xE5\ty u",
    "\xA5\xE7\ty o",
    "\xA5\xEE\tw a",
    "",
};
// 0x008C6654
extern const char* const s_KanaTwo[] = {
    "K2K2\t",
    "\xA5\xAD\xA5\xE3\tk y a",
    "\xA5\xAD\xA5\xE5\tk y u",
    "\xA5\xAD\xA5\xE7\tk y o",
    "\xA5\xB7\xA5\xE3\tsh a",
    "\xA5\xB7\xA5\xE5\tsh u",
    "\xA5\xB7\xA5\xA7\tsh e",
    "\xA5\xB7\xA5\xE7\tsh o",
    "\xA5\xC1\xA5\xE3\tch a",
    "\xA5\xC1\xA5\xE5\tch u",
    "\xA5\xC1\xA5\xA7\tch e",
    "\xA5\xC1\xA5\xE7\tch o",
    "\xA5\xCB\xA5\xE3\tn y a",
    "\xA5\xCB\xA5\xE5\tn y u",
    "\xA5\xCB\xA5\xE7\tn y o",
    "\xA5\xD5\xA5\xA1\tf a",
    "\xA5\xD5\xA5\xA3\tf i",
    "\xA5\xD5\xA5\xA7\tf e",
    "\xA5\xD5\xA5\xA9\tf o",
    "\xA5\xD2\xA5\xE3\th y a",
    "\xA5\xD2\xA5\xE5\th y u",
    "\xA5\xD2\xA5\xE7\th y o",
    "\xA5\xDF\xA5\xE3\tm y a",
    "\xA5\xDF\xA5\xE5\tm y u",
    "\xA5\xDF\xA5\xE7\tm y o",
    "\xA5\xEA\xA5\xE3\tr y a",
    "\xA5\xEA\xA5\xE5\tr y u",
    "\xA5\xEA\xA5\xE7\tr y o",
    "\xA5\xAE\xA5\xE3\tg y a",
    "\xA5\xAE\xA5\xE5\tg y u",
    "\xA5\xAE\xA5\xE7\tg y o",
    "\xA5\xB8\xA5\xE3\tj a",
    "\xA5\xB8\xA5\xE5\tj u",
    "\xA5\xB8\xA5\xA7\tj e",
    "\xA5\xB8\xA5\xE7\tj o",
    "\xA5\xC2\xA5\xE3\tj a",
    "\xA5\xC2\xA5\xE5\tj u",
    "\xA5\xC2\xA5\xA7\tj e",
    "\xA5\xC2\xA5\xE7\tj o",
    "\xA5\xC7\xA5\xA3\td i",
    "\xA5\xC9\xA5\xA5\td u",
    "\xA5\xC7\xA5\xE3\td y a",
    "\xA5\xC7\xA5\xE5\td y u",
    "\xA5\xC7\xA5\xE7\td y o",
    "\xA5\xD3\xA5\xE3\tb y a",
    "\xA5\xD3\xA5\xE5\tb y u",
    "\xA5\xD3\xA5\xE7\tb y o",
    "\xA5\xD4\xA5\xE3\tp y a",
    "\xA5\xD4\xA5\xE5\tp y u",
    "\xA5\xD4\xA5\xE7\tp y o",
    "\xA5\xF4\xA5\xA1\tb a",
    "\xA5\xF4\xA5\xA3\tb i",
    "\xA5\xF4\xA5\xA7\tb e",
    "\xA5\xF4\xA5\xA9\tb o",
    "\xA5\xC6\xA5\xA3\tt i",
    "\xA5\xC8\xA5\xA5\tt u",
    "\xA5\xC4\xA5\xA1\tts a",
    "\xA5\xC4\xA5\xA3\tts i",
    "\xA5\xC4\xA5\xA7\tts e",
    "\xA5\xC4\xA5\xA9\tts o",
    "\xA5\xA6\xA5\xA3\tw i",
    "\xA5\xA6\xA5\xA7\tw e",
    "\xA5\xA6\xA5\xA9\tw o",
    "",
};
// 0x00982EEC
extern const char* const s_RomajiPhones[] = {
    "(0)",
    "N",
    "a",
    "b",
    "ch",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "m",
    "n",
    "o",
    "p",
    "q",
    "r",
    "s",
    "sh",
    "sil",
    "t",
    "ts",
    "u",
    "w",
    "y",
    "z",
    "",
};
// 0x00982D98
extern const char* const s_PinyinPhones[] = {
    "NULL",
    "a",
    "ay",
    "c",
    "cc",
    "ch",
    "e",
    "ey",
    "h",
    "i",
    "k",
    "kh",
    "kk",
    "ks",
    "l",
    "lh",
    "lk",
    "lm",
    "lp",
    "lph",
    "ls",
    "lth",
    "m",
    "n",
    "nc",
    "ng",
    "nh",
    "o",
    "oy",
    "p",
    "ph",
    "pp",
    "ps",
    "s",
    "sil",
    "ss",
    "t",
    "th",
    "tt",
    "u",
    "uy",
    "wa",
    "way",
    "we",
    "wey",
    "wi",
    "wu",
    "ya",
    "yay",
    "ye",
    "yey",
    "yo",
    "yu",
};

// 0x0015B0FC
s32 ApplySoundRules(char* text)
{
    if (ReplaceText(text, "ay", "ANY", "ANY", "A", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ey", "ANY", "ANY", "E", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "oy", "ANY", "ANY", "O", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "uy", "ANY", "ANY", "U", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ch", "ANY", "ANY", "C", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "kh", "ANY", "ANY", "K", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "th", "ANY", "ANY", "T", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ph", "ANY", "ANY", "P", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ng.", "ANY", "ANY", "G", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ng", "ANY", "ANY", "", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lp.", "ANY", "$", "l.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lp.", "ANY", "n|m", "l.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lp.s", "ANY", "ANY", "l.ss", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lp.k", "ANY", "ANY", "l.kk", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lp.c", "ANY", "ANY", "l.cc", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lp.t", "ANY", "ANY", "l.tt", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "nc.", "ANY", "$", "n.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "nc.", "ANY", "n|m", "n.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "nc.s", "ANY", "ANY", "n ss", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "nc.k", "ANY", "ANY", "n.kk", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "nc.t", "ANY", "ANY", "n.tt", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lm.", "ANY", "$", "m.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lm.", "ANY", "n|m", "m.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lm.s", "ANY", "ANY", "m.ss", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lm.k", "ANY", "ANY", "m.kk", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lm.c", "ANY", "ANY", "ay", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lm.t", "ANY", "ANY", "m.tt", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lk.", "ANY", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "l.k", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lk.", "ANY", "h", "l.k", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lk.k", "ANY", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "l.kk", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lk.", "ANY", "ANY", "k.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ks.", "ANY", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "k.s", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ks.", "ANY", "ANY", "k.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ls.", "ANY", "ANY", "l.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ps.", "ANY", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "p.s", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ps.", "ANY", "ANY", "p.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lT.", "ANY", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "l.T", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lT.", "ANY", "ANY", "l.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lP.", "ANY", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "l.P", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lP.", "ANY", "ANY", "p.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "nh.", "ANY", "$", "n.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lh.", "ANY", "$", "l.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "nh.", "ANY", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "n.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lh.", "ANY", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "l.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "nh.", "ANY", "n|l|m|s", "n.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "lh.", "ANY", "n|l|m|s", "l.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "h.", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", ".", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "h.", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "m", ".", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "h.", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "n", "n.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "h.", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "$", "t.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "h.", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "C", "c.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "h.", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "T", "t.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "T.", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "i", "C.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "t.", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "i", "c.", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "t.h", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "i", ".C", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, ".", "ANY", "ANY", "", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "P", "ANY", "$", "p", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "P", "ANY", "c|C|h|k|K|l|m|n|G|p|P|s|t|T", "p", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "T", "ANY", "$", "t", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "T", "ANY", "c|C|h|k|K|l|m|n|G|p|P|s|t|T", "t", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "K", "ANY", "$", "k", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "K", "ANY", "c|C|h|k|K|l|m|n|G|p|P|s|t|T", "k", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "C", "ANY", "$", "c", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "C", "ANY", "c|C|h|k|K|l|m|n|G|p|P|s|t|T", "c", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "kk", "ANY", "$", "k", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "kk", "ANY", "c|C|h|k|K|l|m|n|G|p|P|s|t|T", "k", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ss", "ANY", "$", "t", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "s", "ANY", "$", "t", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "c", "ANY", "$", "t", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ss", "ANY", "k|t|p|h|C|K|T|P|m|n|l", "t", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "s", "ANY", "k|t|p|h|C|K|T|P|m|n|l", "t", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "c", "ANY", "k|t|p|C|K|T|P|m|n|l", "t", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "cs", "ANY", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "ss", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "css", "ANY", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "ss", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "sss", "ANY", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "ss", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "hc", "ANY", "ANY", "C", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "hk", "ANY", "ANY", "K", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ht", "ANY", "ANY", "T", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "hp", "ANY", "ANY", "P", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "kh", "ANY", "ANY", "K", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ph", "ANY", "ANY", "P", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "th", "ANY", "ANY", "T", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ch", "ANY", "ANY", "C", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "hs", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "ANY", "ss", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "p", "ANY", "nm", "m", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "P", "ANY", "nm", "m", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ttt", "ANY", "ANY", "tt", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "sst", "ANY", "ANY", "tt", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "st", "ANY", "ANY", "tt", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "cct", "ANY", "ANY", "tt", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ct", "ANY", "ANY", "tt", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ccc", "ANY", "ANY", "cc", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ssc", "ANY", "ANY", "cc", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "sc", "ANY", "ANY", "cc", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ttc", "ANY", "ANY", "cc", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "tc", "ANY", "ANY", "cc", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "n", "ANY", "k|K", "G", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "T", "ANY", "n|m", "n", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "tt", "ANY", "n|m", "n", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "t", "ANY", "n|m", "n", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ss", "ANY", "n|m", "n", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "s", "ANY", "n|m", "n", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "pp", "ANY", "n|m", "m", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "p", "ANY", "n|m", "m", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "P", "ANY", "n|m", "m", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "kk", "ANY", "n|m", "G", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "k", "ANY", "n|m", "G", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "kc", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "kcc", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "pc", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "pcc", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "tc", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "tcc", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ks", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "kss", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ps", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "pss", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ts", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "tss", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "kp", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "kpp", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "tp", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "tpp", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "kt", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "ktt", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "pt", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "ptt", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "pk", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "pkk", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "tk", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "a|A|e|E|i|o|O|u|U|wa|wA|we|wE|wi|wu|ya|yA|ye|yE|yo|yu", "tkk", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "nl", "ANY", "ANY", "ll", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "ln", "ANY", "ANY", "ll", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "l", "m|G", "ANY", "n", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "kl", "ANY", "ANY", "Gn", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "pl", "ANY", "ANY", "mn", REPLACE_ALL) == -2) {
        return -2;
    }
    if (ReplaceText(text, "U", "h|tt", "ANY", "i", REPLACE_ALL) == -2) {
        return -2;
    }
    return 0;
}
} // namespace asr
