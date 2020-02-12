#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <thread>
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

    while(true) {
        // Read 16 bytes at a time
        char buffer[16];
        if(ifs.read(buffer, 16) || !ifs.eof()) {
            // Print action
            if(buffer[8] - 0 == 4) {
                cout << "Selecting key '" << keycodeToString(buffer[12] - 0) << "'" << endl;
            }
            else if(buffer[8] - 0 == 1) {
                switch (buffer[12] - 0) {
                    case 0: 
                        cout << "Key is released" << endl;
                        break;
                    case 1: 
                        cout << "Key is pressed" << endl;
                        break;
                    case 2: 
                        cout << "Key is held" << endl;
                        break;
                    default: 
                        cout << "An unknown key action occurred." << endl;
                        printBuffer(buffer);
                        exit(1);
                }
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

    return 0;
}