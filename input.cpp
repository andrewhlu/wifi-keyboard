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

long int my_e[50], d[50], temp[50], j;
char en[2];
int t, p1, p2, my_n;

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

int prime(long int pr)
{
   int i;
   j = sqrt(pr);
   for(i = 2; i <= j; i++)
   {
      if(pr % i == 0)
         return 0;
   }
   return 1;
 }

long int cd(long int a)
{
   long int k = 1;
   while(1)
   {
      k = k + t;
      if(k % a == 0)
         return(k/a);
   }
}

void encryption_key(int t){
    int k = 0;
    for(int i = 2; i < t; i++){
        if(t % i == 0)
            continue;
        int flag = prime(i);
        if(flag == 1 && i != p1 && i != p2){
            e[k] = i;
            flag = cd(e[k]);
            if(flag > 0){
                d[k] = flag;
                k++;
            }
            if(k == 99)
                break;
        }
    }
}

//function to encrypt the message
void encrypt(char* m, int n, long int* e)
{
   long int pt, ct, key = e[0], k, len;
   int i = 0;
   len = 2;

   while(i != len)
   {
      pt = m[i];
      pt = pt - 96;
      k = 1;
      for(j = 0; j < key; j++)
      {
         k = k * pt;
         k = k % n;
      }
      temp[i] = k;
      ct= k + 96;
      en[i] = ct;
      i++;
   }
   en[i] = -1;
   cout << "\n\nTHE ENCRYPTED MESSAGE IS\n";
   for(i=0; en[i] != -1; i++)
      cout << en[i];
}

//function to decrypt the message
void decrypt(char* m, int n, long int* e)
{
   long int pt, ct, key = d[0], k;
   int i = 0;
   while(en[i] != -1)
   {
      ct = temp[i];
      k = 1;
      for(j = 0; j < key; j++)
      {
         k = k * ct;
         k = k % n;
      }
      pt = k + 96;
      m[i] = pt;
      i++;
   }
   m[i] = -1;
   cout << "\n\nTHE DECRYPTED MESSAGE IS\n";
   for(i = 0; m[i] != -1; i++)
      cout << m[i];

  cout << endl;
}

int main(int argc, char* argv[]) {
    // Check for valid arguments
    if(argc != 4) {
        printf("Usage %s ip port type('UDP' or 'TCP')\n", argv[0]);
        exit(1);
    }
    // Symmetric key
    char symmetric_key [2];
    symmetric_key[0] = 0x95;
    symmetric_key[1] = 0x6A;

    // Prime number
    p1 = 52267;
    p2 = 67261;

    my_n = p1 * p2;
    t = (p1 - 1) * (p2 - 1);

    // Generate the encryption key
    encryption_key(t);

    encrypt(symmetric_key);

    decrypt(symmetric_key);

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
    if(type == "TCP")
        send(socket_id, my_n, sizeof(my_n), 0);
        send(socket_id, my_e, sizeof(my_e), 0);
    else
        sendto(socket_id, my_n, sizeof(my_n), 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));
        sendto(socket_id, my_e, sizeof(my_e), 0, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));

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