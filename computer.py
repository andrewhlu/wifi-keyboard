import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Setting up connection and bind to a port
# s = socket.socket()
port = 11111
s.bind(('', port))

s.listen(1)      
print "socket is listening"            
  
# a forever loop until we interrupt it or  
# an error occurs 
while True:
   # Establish connection with client. 
   c, addr = s.accept()
   print 'Got connection from', addr 

   #print c.recv(1024)
   packet = c.recv(1024)
   info = b(packet).decode()
   if(info[] == 0)

   else if(info[] == 1)

   else

   

   # Close the connection with the client 
   c.close() 
