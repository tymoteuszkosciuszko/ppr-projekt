#!/usr/bin/env python3
import socket
import base64
import signal
import sys
import xmlrpc.client

# === KONFIGURACJA ===
UDP_HOST = "127.0.0.1"
UDP_PORT = 54321
BUFFER_SIZE = 1
OUTPUT_FILE = "dane_z_php.txt"
RPC_SERVER_URL = "http://127.0.0.1:8000/"

running = True
sock = None

def stop_handler(signum, frame):
    global running
    print("\n[INFO] Przerywanie odbioru danych UDP...")
    running = False

def send_via_xmlrpc(file_path):
    print("[INFO] Łączenie z serwerem Perl przez XML-RPC...")
    server = xmlrpc.client.ServerProxy(RPC_SERVER_URL)

    with open(file_path, 'rb') as f:
        raw_data = f.read()
        b64_data = base64.b64encode(raw_data).decode('utf-8')

    result = server.send_data(b64_data)
    print("[XML-RPC] Odpowiedź serwera:", result)

def udp_receive():
    global sock
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_HOST, UDP_PORT))
    print(f"[INFO] Serwer UDP działa na {UDP_HOST}:{UDP_PORT}")

    with open(OUTPUT_FILE, "wb") as f:
        while running:
            try:
                data, addr = sock.recvfrom(BUFFER_SIZE)
            except KeyboardInterrupt:
                break
            except:
                continue

            if not data:
                continue

            f.write(data)
            f.flush()
            sock.sendto(b'ACK', addr)

    sock.close()

if __name__ == "__main__":
    signal.signal(signal.SIGINT, stop_handler)

    try:
        udp_receive()
        print("[INFO] Zakończono odbieranie UDP.")
        send_via_xmlrpc(OUTPUT_FILE)
    except Exception as e:
        print(f"[BŁĄD] {e}")
        if sock:
            sock.close()
        sys.exit(1)
