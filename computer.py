from socket import *
import win32api
import win32con
import time
import math
import sys
import secrets

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
				227:0x5B,
				231:0x5C,
				101:0x5D,
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
				83:0x90,
				71:0x91,
				225:0xA0,
				229:0xA1,
				224:0xA2,
				228:0xA3,
				226:0xA4,
				230:0xA5,
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
				46:0xBB,
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

def release(*args):
	'''
	release depressed keys
	accepts as many arguments as you want.
	e.g. release('left_arrow', 'a','b').
	'''
	for i in args:
		win32api.keybd_event(VK_CODE[i],0 ,win32con.KEYEVENTF_KEYUP ,0)

def getPrimeNumber():
	primeNum = secrets.randbelow(1000000)
	while not isPrime(primeNum):
		primeNum -= 1
	return primeNum

def isPrime(num):
	for i in range(2, math.floor(math.sqrt(num))):
		if num % i == 0:
			return False
	return True

def getRelativePrime(n, z):
	relativeNum = secrets.randbelow(n)
	while math.gcd(relativeNum, z) > 1:
		relativeNum -= 1
	return relativeNum

def getExactDivisible(e, z):
	 d = 1
	 while ((e * d) % z) != 1:
		  d += 1
	 return d

def encryption(message, n, e):
    return pow(message, e) % n

# Check for input arguments
if len(sys.argv) != 3:
	sys.exit("Usage " + str(sys.argv[0]) + " port type('UDP' or 'TCP')")
else:
	# Create public-private encryption key
	prime1 = getPrimeNumber()
	prime2 = getPrimeNumber()

	keyN = prime1 * prime2
	keyZ = (prime1 - 1) * (prime2 - 2)
	keyE = getRelativePrime(keyN, keyZ)
	keyD = getExactDivisible(keyE, keyZ)
	 
	# Set up the socket connection
	if str(sys.argv[2]) == "TCP":
		print("Setting up TCP socket on port " + str(sys.argv[1]))
		s = socket(AF_INET,SOCK_STREAM)
	elif str(sys.argv[2]) == "UDP":
		print("Setting up UDP socket on port " + str(sys.argv[1]))
		s = socket(AF_INET,SOCK_DGRAM)
	else:
		sys.exit("Unknown type: '" + str(sys.argv[2]) + "', must be either 'UDP' or 'TCP'")

	port = int(sys.argv[1])
	s.bind(('', port))

	if str(sys.argv[2]) == "TCP":
		s.listen(1)
	
	print("socket is listening")

	# Wait for an incoming connection
	print("Waiting for new connection")

	# Keep receiving input until it's over.
	while True:
		if str(sys.argv[2]) == "TCP":
			c, addr = s.accept()
			print("Got connection from " + str(addr[0]) + ", Port " + str(addr[1])) 

		while True:
			if str(sys.argv[2]) == "TCP":
				packet = c.recv(1024)
			elif str(sys.argv[2]) == "UDP":
				message = s.recvfrom(1024)
				packet = message[0]

			packet = int.from_bytes(packet, "big") ^ key

			# Check to see if the connection is still active
			if not packet: 
				print("Connection closed")
				break

			character = math.floor(packet / 256)
			action = packet % 256

			if action == 1:
				pressAndHold(character)
			elif action == 2:
				release(character)
				pressAndHold(character)
			elif action == 3:
				release(character)

			print("Character: " + str(character) + ", Action: " + str(action))
