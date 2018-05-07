#coding=UTF-8  
from socket import *  

HOST = 'localhost'
PORT = 8099
BUFSIZE = 1024
ADDR = (HOST, PORT)

udpCliSock = socket(AF_INET,SOCK_DGRAM)
udpCliSock.connect(ADDR)
while True:
    
    data = input("> ")
    if not data:
        break
    data = data.encode(encoding="utf-8") 
    udpCliSock.sendto(data, ADDR)
    data, ADDR = udpCliSock.recvfrom(BUFSIZE)
    if not data:
        break
    print(data.decode(encoding="utf-8"),'from',ADDR)
    udpCliSock.close()
