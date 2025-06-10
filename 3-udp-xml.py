#!/usr/bin/env python3
import socket
from xmlrpc.client import ServerProxy

HOST = "127.0.0.1"
PORT_UDP = 54321
BUFFER_SIZE = 1

# XML-RPC proxy
RPC_SERVER_URL = "http://127.0.0.1:8080"
proxy = ServerProxy(RPC_SERVER_URL)

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((HOST, PORT_UDP))
    print(f"[INFO] UDP server listening on {HOST}:{PORT_UDP}")

    while True:
        data, addr = sock.recvfrom(BUFFER_SIZE)
        if not data:
            continue

        # Wysyłamy bajt jako lista jednoelementowa (aby zachować typ bajta)
        try:
            proxy.receive_byte(ord(data))  # Konwersja bajtu do liczby całkowitej
        except Exception as e:
            print(f"[ERROR] Failed to send via XML-RPC: {e}")

        # ACK
        sock.sendto(b'ACK', addr)

if __name__ == "__main__":
    main()
