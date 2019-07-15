import socket


HOST, PORT = "10.7.56.122", 5678    


bufSize = 1024


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))
sock.send(b'Hi this is Client:Ashish')

print((sock.recv(bufSize).split(b'\0',1)[0]))



while 1: 
    data = (sock.recv(bufSize)) 
    data = data.split(b'\0',1)[0]
    strData = data.decode("utf-8")
    print("Server:", strData)
    
    if(strData == '#'):
       print("Disconnecting .... ")
       break; 




