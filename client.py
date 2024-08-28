import socket

def log_message(message):
    print(f"[LOG]: {message}")

def send_request(sock, request):
    log_message(f"Sending request to server: {request}")
    sock.sendall(request.encode())

def main():
    server_ip = '127.0.0.1'
    server_port = 8080
    
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((server_ip, server_port))
        log_message("Connected to server")
        
        while True:
            print("\n----------------------------Menu:----------------------------\n:")
            print("1. Generate Username")
            print("2. Generate Password")
            print("3. Exit")
            choice = input("Choose an option: ")
            if choice == '3':
                log_message("Exiting client")
                break
            if choice == '1':
                length = input("Enter the desired length: ")
                if not length.isdigit():
                    log_message("Invalid length input")
                    print("Error: Length must be a number.")
                    continue
                length = int(length)
                request = f"username {length}"
            elif choice == '2':
                length = input("Enter the desired length: ")
                if not length.isdigit():
                    log_message("Invalid length input")
                    print("Error: Length must be a number.")
                    continue
                length = int(length)
                request = f"password {length}"
            else:
                log_message("Invalid choice")
                print("Error: Invalid choice. Please select 1, 2, or 3.")
                continue
            send_request(sock, request)
            response = sock.recv(1024).decode()
            log_message("Received response from server")
            print(f"Response: {response}")
        log_message("Client shutdown")

main()