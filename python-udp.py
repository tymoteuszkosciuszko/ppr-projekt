#!/usr/bin/env python3
import socket
import base64

HOST = "127.0.0.1"
PORT = 54321
BUFFER_SIZE = 1
OUTPUT_FILE = "dane_z_php.txt"

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((HOST, PORT))
    print(f"[INFO] UDP server listening on {HOST}:{PORT}")

    with open(OUTPUT_FILE, "wb") as f:
        while True:
            data, addr = sock.recvfrom(BUFFER_SIZE)
            if not data:
                continue  # ignorujemy puste pakiety

            f.write(data)
            f.flush()  # natychmiastowy zapis do pliku
            sock.sendto(b'ACK', addr)

if __name__ == "__main__":
    main()
