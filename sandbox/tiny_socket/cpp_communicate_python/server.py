import torch
import traceback
import socket
import json

host = "127.0.0.1"
port = 6009

conn = None
addr = None

listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

def init(wish_host, wish_port):
    global host, port, listener
    host = wish_host
    port = wish_port
    listener.bind((host, port))
    listener.listen()
    listener.settimeout(0)

def try_connect():
    global conn, addr, listener
    try:
        conn, addr = listener.accept()
        print(f"\nConnected by {addr}")
        conn.settimeout(None)
    except Exception as inst:
        pass
            
def read():
    global conn
    messageLength = conn.recv(4)
    messageLength = int.from_bytes(messageLength, 'little')
    message = conn.recv(messageLength)
    return message

def send(str):
    global conn
    if str != None:
        message_bytes = bytes(json.dumps(send_data),'ascii')
        conn.sendall(len(message_bytes).to_bytes(4, 'little'))
        conn.sendall(message_bytes)

init("127.0.0.1",6009)
while True:
    if conn == None:
        try_connect()
    while conn != None:
        try:
            message = read()
            message = json.loads(message.decode("utf-8"))
            observation = torch.tensor(message["observation"])
            print(f"receive {observation} from client")

            action = torch.rand(3)
            send_data = {}
            send_data['action'] = action.numpy().tolist()
            send(json.dumps(send_data))
            print(f"send {action} to client")
            break
        except Exception as e:
            conn = None