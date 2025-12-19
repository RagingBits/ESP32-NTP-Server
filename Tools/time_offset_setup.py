import socket
import argparse

def send_udp_message(ip, port, offset):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    message = f"1234B\n{offset}\n"
    sock.sendto(message.encode(), (ip, port))
    sock.close()
    print(f"Sent: {message} -> {ip}:{port}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Send SSID and PASSWORD via UDP")
    parser.add_argument("--offset", required=True, help="time offset in hours -23 to +23")
    parser.add_argument("--ip", default="192.168.1.2", help="Target IP address")
    parser.add_argument("--port", type=int, default=8080, help="Target UDP port")

    args = parser.parse_args()

    send_udp_message(args.ip, args.port, args.offset)