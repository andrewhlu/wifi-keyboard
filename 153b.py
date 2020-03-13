import serial
import win32api
import win32con
import time

# https://gist.github.com/chriskiehl/2906125
VK_CODE = {'backspace':0x08,
           'tab':0x09,
           'clear':0x0C,
           'enter':0x0D,
           'shift':0x10,
           'ctrl':0x11,
           'alt':0x12,
           'pause':0x13,
           'caps_lock':0x14,
           'esc':0x1B,
           'spacebar':0x20,
           'page_up':0x21,
           'page_down':0x22,
           'end':0x23,
           'home':0x24,
           'left_arrow':0x25,
           'up_arrow':0x26,
           'right_arrow':0x27,
           'down_arrow':0x28,
           'select':0x29,
           'print':0x2A,
           'execute':0x2B,
           'print_screen':0x2C,
           'ins':0x2D,
           'del':0x2E,
           'help':0x2F,
            39:0x30,
            30:0x31,
            31:0x32,
            32:0x33,
            33:0x34,
            34:0x35,
            35:0x36,
            36:0x37,
            37:0x38,
            38:0x39,
            4:0x41,
            5:0x42,
            6:0x43,
            7:0x44,
            8:0x45,
            9:0x46,
            10:0x47,
            11:0x48,
            12:0x49,
            13:0x4A,
            14:0x4B,
            15:0x4C,
            16:0x4D,
            17:0x4E,
            18:0x4F,
            19:0x50,
            20:0x51,
            21:0x52,
            22:0x53,
            23:0x54,
            24:0x55,
            25:0x56,
            26:0x57,
            27:0x58,
            28:0x59,
            29:0x5A,
            98:0x60,
            89:0x61,
            90:0x62,
            91:0x63,
            92:0x64,
            93:0x65,
            94:0x66,
            95:0x67,
            96:0x68,
            97:0x69,
            85:0x6A,
            87:0x6B,
           'separator_key':0x6C,
            86:0x6D,
            99:0x6E,
            84:0x6F,
            58:0x70,
            59:0x71,
            60:0x72,
            61:0x73,
            62:0x74,
            63:0x75,
            64:0x76,
            65:0x77,
            66:0x78,
            67:0x79,
            68:0x7A,
            69:0x7B,
            104:0x7C,
            105:0x7D,
            106:0x7E,
            107:0x7F,
            108:0x80,
            109:0x81,
            110:0x82,
            111:0x83,
            112:0x84,
            113:0x85,
            114:0x86,
            115:0x87,
           'num_lock':0x90,
           'scroll_lock':0x91,
           'left_shift':0xA0,
           'right_shift ':0xA1,
           'left_control':0xA2,
           'right_control':0xA3,
           'left_menu':0xA4,
           'right_menu':0xA5,
           'browser_back':0xA6,
           'browser_forward':0xA7,
           'browser_refresh':0xA8,
           'browser_stop':0xA9,
           'browser_search':0xAA,
           'browser_favorites':0xAB,
           'browser_start_and_home':0xAC,
           'volume_mute':0xAD,
           'volume_Down':0xAE,
           'volume_up':0xAF,
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
           ',':0xBC,
           '-':0xBD,
           '.':0xBE,
           '/':0xBF,
           '`':0xC0,
           ';':0xBA,
           '[':0xDB,
           '\\':0xDC,
           ']':0xDD,
           "'":0xDE,
           '`':0xC0}

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