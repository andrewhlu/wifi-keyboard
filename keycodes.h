#ifndef KEYCODES_H
#define KEYCODES_H

#include <stdint.h>

const char* keycodes[] = {
    "No key pressed",
    "Error Roll Over", // 1
    "err",
    "err",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G", // 10
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q", // 20
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "1", // 30
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "Enter", // 40
    "Esc",
    "Backspace",
    "Tab",
    "Space",
    "- / _",
    "= / +",
    "[ / {",
    "] / }",
    "\\ / |",
    "Non-US # and ~", // 50
    "; / :",
    "' / \"",
    "` / ~",
    ", / <",
    ". / >",
    "/ / ?",
    "Caps Lock",
    "F1",
    "F2",
    "F3", // 60
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "Print Screen", // 70
    "Scroll Lock",
    "Pause",
    "Insert",
    "Home",
    "Page Up",
    "Delete",
    "End",
    "Page Down",
    "Right",
    "Left", // 80
    "Down",
    "Up",
    "Num Lock",
    "Keypad /",
    "Keypad *",
    "Keypad -",
    "Keypad +",
    "Keypad Enter",
    "Keypad 1 / End",
    "Keypad 2 / Down", // 90
    "Keypad 3 / Page Down",
    "Keypad 4 / Left",
    "Keypad 5",
    "Keypad 6 / Right",
    "Keypad 7 / Home",
    "Keypad 8 / Up",
    "Keypad 9 / Page Up",
    "Keypad 0 / Insert",
    "Keypad . / Delete",
    "Non-US \\ and |", // 100
    "Applic",
    "Power",
    "Keypad =",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19", // 110
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "Execute",
    "Help",
    "Menu",
    "Select",
    "Stop", // 120
    "Again",
    "Undo",
    "Cut",
    "Copy",
    "Paste",
    "Find",
    "Mute",
    "Volume Up",
    "Volume Down",
    "Locking Caps Lock", // 130
    "Locking Num Lock",
    "Locking Scroll Lock",
    "Keypad ,",
    "Keypad =",
    "International 1",
    "International 2",
    "International 3",
    "International 4",
    "International 5",
    "International 6", // 140
    "International 7",
    "International 8",
    "International 9",
    "LANG 1",
    "LANG 2",
    "LANG 3",
    "LANG 4",
    "LANG 5",
    "LANG 6",
    "LANG 7", // 150
    "LANG 8",
    "LANG 9",
    "Alt Erase",
    "SysRq",
    "Cancel",
    "Clear",
    "Prior",
    "Return",
    "Separ",
    "Out", // 160
    "Oper",
    "Clear / Again",
    "CrSel / Props",
    "ExSel",
    "",
    "",
    "",
    "",
    "",
    "", // 170
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "", // 180
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "", // 190
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "", // 200
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "", // 210
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "", // 220
    "",
    "",
    "",
    "Left Ctrl",
    "Left Shift",
    "Left Alt",
    "Left GUI",
    "Right Ctrl",
    "Right Shift",
    "Right Alt", // 230
    "Right GUI",
    "Media Play / Pause",
    "Media Stop",
    "Media Previous",
    "Media Next",
    "Media Eject",
    "Media Volume Up",
    "Media Volume Down",
    "Media Mute",
    "Media WWW", // 240
    "Media Back",
    "Media Forward",
    "Media Stop",
    "Media Find",
    "Media Scroll Up",
    "Media Scroll Down",
    "Media Edit",
    "Media Sleep",
    "Media Coffee",
    "Media Refresh", // 250
    "Media Calc",
    "",
    "",
    "",
    ""
};

const char* keycodeToString(uint8_t code) {
    return keycodes[code];
}

#endif // KEYCODES_H