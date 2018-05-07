#-*- coding:utf-8 -*- 
from socket import *
from time import ctime

HOST = ''
PORT = 8099
BUFSIZE = 1024
ADDR = (HOST, PORT)
status=0

udpServer = socket(AF_INET,SOCK_DGRAM)
udpServer.bind(ADDR)

while True:
    print("waiting for connection...")
    data, ADDR = udpServer.recvfrom(BUFSIZE)
    data = data.decode(encoding='utf-8')
    #data = "at %s :%s "%(ctime(),data)  
    rec_buff=data.split(';')
    status=rec_buff[1]
    if status[2]=='1':
        udpServer.sendto(data.encode('utf-8'),ADDR)
        print("connected sucessed...recevied from：", ADDR)
        print (rec_buff[0])
        print (rec_buff[1])
        print('info:',rec_buff[2])
    elif status[2]=='2':
        udpServer.sendto(data.encode('utf-8'),ADDR)
        print('download sucessed')
    elif status[2]=='3':
        print('download failed')
    
    
udpServer.close()
