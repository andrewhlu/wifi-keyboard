from socket import *

# Set up TCP connection and bind to a port
s = socket(AF_INET,SOCK_STREAM)
port = 8080
s.bind(('', port))
s.listen(1)      
print "socket is listening"

# Wait for an incoming connection
print "Waiting for new connection"

# Keep receiving input until it's over.
while True:
   c, addr = s.accept()
   print 'Got connection from', addr 

   while True:
      packet = c.recv(1024)

      # Check to see if the connection is still active
      if not packet: 
         print "Connection closed"
         break

      character = ord(packet[0])
      action = ord(packet[1])

      print "Character: " + str(character) + ", Action: " + str(action)
