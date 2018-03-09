import socket
import time
import sys
#RPi's IP
SERVER_IP = "10.133.211.162"
SERVER_PORT = 1234

print("Starting socket: TCP...")
server_addr = (SERVER_IP, SERVER_PORT)
socket_tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

while True:
    try:
        print("Connecting to server @ %s:%d..." %(SERVER_IP, SERVER_PORT))
        socket_tcp.connect(server_addr)
        break
    except Exception:
        print("Can't connect to server,try it latter!")
        time.sleep(1)
        continue
print("Please input 1 or 0 to turn on/off the led!")
while True:
    try:
        data = socket_tcp.recv(512)
        if len(data)>0:
            print("Received: %s" % data)
            command=raw_input()
            socket_tcp.send(command)
            time.sleep(1)
            continue
    except Exception:
        socket_tcp.close()
        socket_tcp=None
        sys.exit(1)
