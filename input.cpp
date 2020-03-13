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
    // Check for valid arguments
    if(argc != 4) {
        printf("Usage %s ip port\n", argv[0]);
        exit(1);
    }

    // Set up socket1
    char* server_ip = argv[1];
    int port = atoi(argv[2]);
    string type = argv[3];
    // If the connection is TCP
    if(type == "TCP"){ 
        int socket_id = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr(server_ip);
        server_address.sin_port = htons(port);

        // Initialize socket
        int connection_socket_id = connect(socket_id, (struct sockaddr *)&server_address, sizeof(server_address));
    }
    // If the connection is UDP
    else if(type == "UDP"){
        int socket_id = socket(AF_INET, SOCK_DGRAM, 0);

        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr(server_ip);
        server_address.sin_port = htons(port);

        bind(socket_id, (struct sockaddr *)&server_address, sizeof(server_address));
    }
    else{
        cout << "Invalid Argument" << endl;
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
                message[0] = keyToSend;
                message[1] = actionToSend;

                // Send message
                if(type == "TCP")
                    send(socket_id, message, sizeof(message), 0);
                else
                    sendto(socket_id, message, sizeof(message), 0);
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