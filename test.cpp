#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <thread>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "keycodes.h"
#include "math.h"
using namespace std;

//function to encrypt the message
void encrypt(char* message, int* encrypted, int n, int key, int len) {
    for(int i = 0; i < len; i++) {
        int k = 1;

        for(int j = 0; j < key; j++) {
            k = (k * (message[i] - 0)) % n;
        }

        cout << "Original: " << message[i] - 0 << ", K: " << k << endl;
        encrypted[i] = k;
    }

    encrypted[len] = -1;

    cout << "Encrypted Message: '";
    for(int i = 0; encrypted[i] != -1; i++) {
        cout << encrypted[i] << " ";
    }
    cout << "'" << endl;
}

//function to decrypt the message
void decrypt(int* encrypted, char* decrypted, int n, int key, int len) {
    for(int i = 0; i < len; i++) {
        int k = 1;

        for(int j = 0; j < key; j++) {
            k = (k * encrypted[i]) % n;
        }

        cout << "K: " << k << endl;
        decrypted[i] = k;
    }
    
    decrypted[len] = -1;

    cout << "Decrypted Message: '";
    for(int i = 0; decrypted[i] != -1; i++) {
        cout << decrypted[i];
    }
    cout << "'" << endl;
}

int main(int argc, char *argv[]) {
    int newE = 211;
    int newN = 232;
    int newD = 183;

    char message[1024] = "abcdefghijklmnop";
    int encrypted[1024] = {0};
    char decrypted[1024];
    memset(encrypted, 0, sizeof(encrypted));
    memset(decrypted, 0, sizeof(decrypted));

    encrypt(message, encrypted, newN, newE, strlen(message));
    decrypt(encrypted, decrypted, newN, newD, strlen(message));
}