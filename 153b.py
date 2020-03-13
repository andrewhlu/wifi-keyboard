import serial
import win32api
import win32con
import time

# https://gist.github.com/chriskiehl/2906125
VK_CODE = {42:0x08,
           43:0x09,
           156:0x0C,
           40:0x0D,
           'shift':0x10,
           'ctrl':0x11,
           'alt':0x12,
           72:0x13,
           57:0x14,
           41:0x1B,
           44:0x20,
           75:0x21,
           78:0x22,
           77:0x23,
           74:0x24,
           80:0x25,
           82:0x26,
           79:0x27,
           81:0x28,
           119:0x29,
           'print':0x2A,
           116:0x2B,
           70:0x2C,
           73:0x2D,
           76:0x2E,
           117:0x2F,
           '0':0x30,
           '1':0x31,
           '2':0x32,
           '3':0x33,
           '4':0x34,
           '5':0x35,
           '6':0x36,
           '7':0x37,
           '8':0x38,
           '9':0x39,
           'a':0x41,
           'b':0x42,
           'c':0x43,
           'd':0x44,
           'e':0x45,
           'f':0x46,
           'g':0x47,
           'h':0x48,
           'i':0x49,
           'j':0x4A,
           'k':0x4B,
           'l':0x4C,
           'm':0x4D,
           'n':0x4E,
           'o':0x4F,
           'p':0x50,
           'q':0x51,
           'r':0x52,
           's':0x53,
           't':0x54,
           'u':0x55,
           'v':0x56,
           'w':0x57,
           'x':0x58,
           'y':0x59,
           'z':0x5A,
           'numpad_0':0x60,
           'numpad_1':0x61,
           'numpad_2':0x62,
           'numpad_3':0x63,
           'numpad_4':0x64,
           'numpad_5':0x65,
           'numpad_6':0x66,
           'numpad_7':0x67,
           'numpad_8':0x68,
           'numpad_9':0x69,
           'multiply_key':0x6A,
           'add_key':0x6B,
           'separator_key':0x6C,
           'subtract_key':0x6D,
           'decimal_key':0x6E,
           'divide_key':0x6F,
           'F1':0x70,
           'F2':0x71,
           'F3':0x72,
           'F4':0x73,
           'F5':0x74,
           'F6':0x75,
           'F7':0x76,
           'F8':0x77,
           'F9':0x78,
           'F10':0x79,
           'F11':0x7A,
           'F12':0x7B,
           'F13':0x7C,
           'F14':0x7D,
           'F15':0x7E,
           'F16':0x7F,
           'F17':0x80,
           'F18':0x81,
           'F19':0x82,
           'F20':0x83,
           'F21':0x84,
           'F22':0x85,
           'F23':0x86,
           'F24':0x87,
           83:0x90,
           71:0x91,
           225:0xA0,
           229:0xA1,
           224:0xA2,
           230:0xA3,
           'left_menu':0xA4,
           'right_menu':0xA5,
           'browser_back':0xA6,
           'browser_forward':0xA7,
           'browser_refresh':0xA8,
           'browser_stop':0xA9,
           'browser_search':0xAA,
           'browser_favorites':0xAB,
           'browser_start_and_home':0xAC,
           127:0xAD,
           129:0xAE,
           128:0xAF,
           'next_track':0xB0,
           'previous_track':0xB1,
           'stop_media':0xB2,
           'play/pause_media':0xB3,
           'start_mail':0xB4,
           'select_media':0xB5,
           'start_application_1':0xB6,
           'start_application_2':0xB7,
           'attn_key':0xF6,
           'crsel_key':0xF7,
           'exsel_key':0xF8,
           'play_key':0xFA,
           'zoom_key':0xFB,
           'clear_key':0xFE,
           '+':0xBB,
           54:0xBC,
           45:0xBD,
           55:0xBE,
           56:0xBF,
           53:0xC0,
           51:0xBA,
           47:0xDB,
           49:0xDC,
           48:0xDD,
           52:0xDE}

def pressAndHold(*args):
    '''
    press and hold. Do NOT release.
    accepts as many arguments as you want.
    e.g. pressAndHold('left_arrow', 'a','b').
    '''
    for i in args:
        win32api.keybd_event(VK_CODE[i], 0,0,0)
        # time.sleep(.05)

def release(*args):
    '''
    release depressed keys
    accepts as many arguments as you want.
    e.g. release('left_arrow', 'a','b').
    '''
    for i in args:
        win32api.keybd_event(VK_CODE[i],0 ,win32con.KEYEVENTF_KEYUP ,0)

ser = serial.Serial(
    port='COM10',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        timeout=0)

print("connected to: " + ser.portstr)

direction = None
pressed = None

# shell = win32com.client.Dispatch("WScript.Shell")

while True:
    buffer = ser.read()

    if(len(buffer) > 0):
        if direction == None:
            direction = buffer.decode("ascii")
        elif pressed == None:
            pressed = buffer.decode("ascii")

            # We now have everything we need to emulate keypress
            if direction == "U":
                if pressed == "P":
                    pressAndHold('w')
                elif pressed == "R":
                    release('w')
            elif direction == "D":
                if pressed == "P":
                    pressAndHold('s')
                elif pressed == "R":
                    release('s')
            elif direction == "L":
                if pressed == "P":
                    pressAndHold('a')
                elif pressed == "R":
                    release('a')
            elif direction == "R":
                if pressed == "P":
                    pressAndHold('d')
                elif pressed == "R":
                    release('d')
            
            # Clear direction and pressed
            direction = None
            pressed = None

# Theoretically, this should never happen
ser.close()