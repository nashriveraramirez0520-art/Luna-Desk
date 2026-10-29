import json
from datetime import datetime
from serial.tools import list_ports

import psutil
import argparse
import serial


def read_stats():
    memory = psutil.virtual_memory()

    return {
        "type": "stats",
        "version": 1,
        "time": datetime.now().astimezone().isoformat(timespec="seconds"),
        "cpu_percent": round(psutil.cpu_percent(interval=1), 1),
        "ram_percent": round(memory.percent, 1),
        "ram_used_gib": round(
            (memory.total - memory.available) / (1024 ** 3), 2
        ),
        "ram_total_gib": round(memory.total / (1024 ** 3), 2),
    }

def show_ports():
    ports = list_ports.comports()
    if not ports:
        print("No serial ports found. Connect Luna Desk and try again.")
        return

    for port in ports:
        print(port.device, "-", port.description)

def parse_arguments():
    parser = argparse.ArgumentParser(description="Luna Desk PC Companion")
    parser.add_argument("--port", help="Serial port, for example COM3")
    return parser.parse_args()

def open_connection(port):
    try:
        return serial.serial_for_url(
            port,
            baudrate=115200,
            timeout=1,
            write_timeout=1,
        )
    except serial.SerialException as error:
        print(f"Could not open {port}: {error}")
        return None

def choose_state(stats):
    if stats ["cpu_percent"] >= 30:
        return "working"
    return "idle"

def main():
    args = parse_arguments()
    print("Selected port:", args.port)
    if args.port == "loop://":
        print("Loopback test mode — no physical device connected.")
    else:
        show_ports()
    connection = None

    if args.port:
        connection = open_connection(args.port)
        if connection is None:
            return
    print("Luna Desk — reading PC stats. Press Ctrl+C to stop.")

    try:
        while True:
            stats = read_stats()
            stats["state"] = choose_state(stats)
            message = json.dumps(stats)
            packet = (message + "\n").encode("utf-8")
            if connection is not None:
                connection.write(packet)
                if args.port == "loop://":
                    received = connection.readline()
                    print("Loopback matches:", received == packet)
                    decoded = received.decode("utf-8")
                    received_stats = json.loads(decoded)
                    print("Received CPU:", received_stats["cpu_percent"])
            print(message, flush=True)
    except KeyboardInterrupt:
        print("\nLuna Desk stopped.")
    except serial.SerialException as error:
        print(f"Serial connection failed: {error}")
    finally:
        if connection is not None:
            connection.close()


if __name__ == "__main__":
    main()