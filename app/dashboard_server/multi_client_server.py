import socket
import threading

PORT = 9090
HOST = '0.0.0.0'

def handle_client(conn, addr):
    print(f"[+] New connection from {addr}")
    data = conn.recv(1024)
    if data:
        print(f"[{addr}] Received: {data.decode().strip()}")
        conn.sendall(b"ACK\n")
    conn.close()

def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((HOST, PORT))
    s.listen()
    print(f"[+] Server listening on {HOST}:{PORT}")

    while True:
        conn, addr = s.accept()
        threading.Thread(target=handle_client, args=(conn, addr)).start()

if __name__ == "__main__":
    main()
