#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <ctime>
using namespace std;

char getRandomChar(){
    int random = rand() % 256;
    return random - '\0';
}

void convertToChar(int src, char* dst) {
    dst[0] = src / 256;
    dst[1] = src % 256;
}

void convertToCharArray(int* src, char* dst, int lenIntArr) {
    for(int i = 0; i < lenIntArr; i++) {
        convertToChar(src[i], &(dst[2*i]));
    }
}

int convertToInt(char* src) {
    return (src[0] - 0) * 256 + (src[1] - 0);
}

bool isPrime(int num) {
    if(num <= 3) {
        return num > 1;
    }
    else if(num % 2 == 0 || num % 3 == 0) {
        return false;
    }

    int i = 5;

    while(i*i <= num) {
        if(num % i == 0 || num % (i + 2) == 0) {
            return false;
        }
        i += 6;
    }

    return true;
}

int getPrimeNumber() {
    int primeNum = rand() % 100;
    while(!isPrime(primeNum)) {
        primeNum--;
    }
    return primeNum;
}

int gcd(int a, int b) {
    int t;
    while(true) {
        t = a % b;
        if(t == 0) {
            return b;
        }
        a = b;
        b = t;
    }
}

int getRelativePrime(int n, int z) {
    int relativeNum = rand() % n;
    while(gcd(relativeNum, z) > 1) {
        relativeNum--;
    }
    return relativeNum;
}

int getExactDivisible(int e, int z) {
    int d = 1;
    while(((e * d) % z) != 1) {
        d++;
    } 
    return d;
}

// Function to encrypt the message
void encrypt(char* message, int* encrypted, int n, int key, int len) {
    for(int i = 0; i < len; i++) {
        int k = 1;

        for(int j = 0; j < key; j++) {
            k = (k * (message[i] - 0)) % n;
        }

        encrypted[i] = k;
    }

    cout << "Encrypted Message: '";
    for(int i = 0; i < len; i++) {
        cout << encrypted[i] << " ";
    }
    cout << "'" << endl;
}

// Function to decrypt the message
void decrypt(int* encrypted, char* decrypted, int n, int key, int len) {
    for(int i = 0; i < len; i++) {
        int k = 1;

        for(int j = 0; j < key; j++) {
            k = (k * encrypted[i]) % n;
        }

        decrypted[i] = k;
    }

    cout << "Decrypted Message: '";
    for(int i = 0; i < len; i++) {
        cout << decrypted[i];
    }
    cout << "'" << endl;
}

// int main(int argc, char *argv[]) {
//     int newE = 885;
//     int newN = 2581;
//     int newD = 2077;

//     char message[1024] = "abcdefghijklmnop";
//     int encrypted[1024] = {0};
//     char decrypted[1024];
//     memset(encrypted, 0, sizeof(encrypted));
//     memset(decrypted, 0, sizeof(decrypted));

//     encrypt(message, encrypted, newN, newE, strlen(message));
//     decrypt(encrypted, decrypted, newN, newD, strlen(message));
// }