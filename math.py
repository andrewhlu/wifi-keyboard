import secrets
import math

def getPrimeNumber():
   primeNum = secrets.randbelow(100)
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

def powerMod(x, y, mod):
    ret = 1
    for i in range(0, y):
        ret = ret * x % mod
    return ret

def encryption(message, n, e):
    m = 0
    for c in message:
        m = (m * 256) + ord(c)
    print("M: " + str(m))
    return powerMod(m, e, n)

def decryption(message, n, e):
    intMessage = powerMod(message, e, n)
    m = ""
    while intMessage > 0:
        m += chr(intMessage % 256)
        intMessage = math.floor(intMessage / 256)
    return m

prime1 = getPrimeNumber()
prime2 = getPrimeNumber()

keyN = prime1 * prime2
keyZ = (prime1 - 1) * (prime2 - 2)
keyE = getRelativePrime(keyN, keyZ)
keyD = getExactDivisible(keyE, keyZ)

print("E: " + str(keyE))
print("N: " + str(keyN))

message = "Hello!"
encrypted = encryption(message, keyN, keyE)
print(encrypted)
decrypted = decryption(encrypted, keyN, keyD)
print(decrypted)