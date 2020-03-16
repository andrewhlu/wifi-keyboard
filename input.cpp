#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <thread>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include "keycodes.h"
#include "math.h"
#include "encryption.h"
using namespace std;

void startKeylog() {
    // Log all keyboard events
    system("cat /dev/input/event0 > /home/pi/wifi-keyboard/test.log");
}

void printBuffer(char buffer[]) {
    cout << "Buffer values: '";
    for(size_t i = 0; i < 16; i++) {
        cout << buffer[i] - 0 << " ";
    }
    cout << "'" << endl;
}

int main(int argc, char* argv[]) {
    srand(time(0));
    // Check for valid arguments
    if(argc != 4) {
        printf("Usage %s ip port type('UDP' or 'TCP')\n", argv[0]);
        exit(1);
    }
    // Symmetric key
    char symmetric_key [2];
    symmetric_key[0] = getRandomChar();
    symmetric_key[1] = getRandomChar();

    cout << "The symmetric key is " << symmetric_key[0] - 0 << " " << symmetric_key[1] - 0 << endl;

    int prime1 = getPrimeNumber();
    int prime2 = getPrimeNumber();

    cout << "Prime Numbers: " << prime1 << ", " << prime2 << endl;

    int my_n = prime1 * prime2;
    int my_z = (prime1 - 1) * (prime2 - 1);
    int my_e = getRelativePrime(my_n, my_z);
    int my_d = getExactDivisible(my_e, my_z);

    cout << "N: " << my_n << ", Z: " << my_z << ", E: " << my_e << ", D: " << my_d << endl;

    char keyboard_n[2];
    char keyboard_e[2];
    convertToChar(my_n, keyboard_n);
    convertToChar(my_e, keyboard_e);

    char computer_n[2] = {0};
    char computer_e[2] = {0};

    // Set up socket1
    char* server_ip = argv[1];
    int port = atoi(argv[2]);
    string type = argv[3];

    int socket_id = 0;
    int connection_socket_id = 0;

    // If the connection is TCP
    if(type == "TCP"){ 
        socket_id = socket(AF_INET, SOCK_STREAM, 0);
    }
    // If the connection is UDP
    else if(type == "UDP"){
        socket_id = socket(AF_INET, SOCK_DGRAM, 0);
    }
    else{
        cout << "Invalid Type Argument: '" << type << "'" << endl;
        exit(1);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(port);

    struct sockaddr_in client_address;
    unsigned int return_len;

    if(type == "TCP"){ 
        // Initialize socket
        connection_socket_id = connect(socket_id, (struct sockaddr *)&server_address, sizeof(server_address));
    }

    // First, remove the existing log
    system("rm /home/pi/wifi-keyboard/test.log");

    // Create a new log file
    system("touch /home/pi/wifi-keyboard/test.log");
    
    // Start keylogger thread
    thread keylog(startKeylog);
    cout << "Keylog thread started" << endl;
    keylog.detach();

    // Open log file
    ifstream ifs("/home/pi/wifi-keyboard/test.log", ios_base::binary);

    // Check if file was opened successfully
    if(!ifs.is_open()) {
        cout << "ERROR: Could not open log file" << endl;
        exit(1);
    }

    // Start by seeking to the end of the file
    ifs.seekg(0, ifs.end);
    streampos position = ifs.tellg();

    // Send (n,e) 
    if(type == "TCP"){
        send(socket_id, keyboard_n, sizeof(keyboard_n), 0);
        send(socket_id, keyboard_e, sizeof(keyboard_e), 0);
    }
    else{
        sendto(socket_id, keyboard_n, sizeof(keyboard_n), 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));
        sendto(socket_id, keyboard_e, sizeof(keyboard_e), 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));
    }
    if(type == "TCP"){
        read(socket_id, computer_n, sizeof(computer_n));
        read(socket_id, computer_e, sizeof(computer_e));
    }
    else{
        recvfrom(socket_id, computer_n, sizeof(computer_n), 0, (struct sockaddr *)&client_address, &return_len);
    }

    char f_encrypted_m[2] = {0};
    char s_encrypted_m[2] = {0};
    
    cout << "My N is " << my_n << endl;
    cout << "My E is " << my_e << endl;
    encrypt(symmetric_key, f_encrypted_m, my_n, my_d, 2);
    encrypt(f_encrypted_m, s_encrypted_m, my_n, my_e, 2);
    cout << "Computer N is " << convertToInt(computer_n) << endl;
    cout << "Computer E is " << convertToInt(computer_e) << endl;
    // encrypt(f_encrypted_m,s_encrypted_m, convertToInt(computer_n), convertToInt(computer_e), 2);

    // if(type == "TCP"){
    //     send(socket_id, s_encrypted_m, sizeof(s_encrypted_m), 0);
    // }
    // else{
    //     sendto(socket_id, s_encrypted_m, sizeof(s_encrypted_m), 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));
    // }

    if(type == "TCP"){
        send(socket_id, f_encrypted_m, sizeof(f_encrypted_m), 0);
    }
    else{
        sendto(socket_id, f_encrypted_m, sizeof(f_encrypted_m), 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));
    }
    // Create buffer to hold the message to send
    char keyToSend = 0;
    char actionToSend = 0;

    while(true) {
        // Read 16 bytes at a time
        char buffer[16];
        if(ifs.read(buffer, 16) || !ifs.eof()) {
            // Print action
            if(buffer[8] - 0 == 4) {
                cout << "Selecting key '" << keycodeToString(buffer[12] - 0) << "'" << endl;
                // Clear keyToSend
                memset(&keyToSend, 0, sizeof(keyToSend));
                keyToSend = buffer[12] - 0;
            }
            else if(buffer[8] - 0 == 1) {
                // Clear actionToSend
                memset(&actionToSend, 0, sizeof(actionToSend));
                switch (buffer[12] - 0) {
                    case 0: 
                        cout << "Key is released" << endl;
                        actionToSend = 3;
                        break;
                    case 1: 
                        cout << "Key is pressed" << endl;
                        actionToSend = 1;
                        break;
                    case 2: 
                        cout << "Key is held" << endl;
                        actionToSend = 2;
                        break;
                    default: 
                        cout << "An unknown key action occurred." << endl;
                        printBuffer(buffer);
                        exit(1);
                }

                // Set up message for sending
                char message[2];
                memset(message, 0, sizeof(message));
                // sprintf(message, "%c%c", keyToSend, actionToSend);

                // Create the buffer and encrypt it with bit-wise XOR
                message[0] = keyToSend ^ symmetric_key[0];
                message[1] = actionToSend ^ symmetric_key[1];

                // Send message
                if(type == "TCP")
                    send(socket_id, message, sizeof(message), 0);
                else
                    sendto(socket_id, message, sizeof(message), 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));
            }
            else if(buffer[8] - 0 == 17) {
                // This corresponds to a Caps Lock or Num Lock command.
                // Bit 12 tells you what the new value is
            }
            else if(buffer[8] - 0 == 0) {
                // Do nothing
            }
            else {
                // We don't know how to handle this action!
                cout << "An unknown action occurred." << endl;
                printBuffer(buffer);
                exit(1);
            }

            // Save new position
            position = ifs.tellg();
        }
        else {
            // Read failed, clear buffer
            ifs.clear();

            // Seek back to last end position
            ifs.seekg(position);
        }
    }
    
    close(connection_socket_id);
    close(socket_id);

    return 0;
}