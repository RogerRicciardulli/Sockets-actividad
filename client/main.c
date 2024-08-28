#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 8080
#define BUFFER_SIZE 256

void log_message(const char *message) {
    printf("[LOG]: %s\n", message);
}

void send_request(SOCKET sock, const char *request) {
    log_message("Sending request to server...");
    send(sock, request, strlen(request), 0);
}

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    int choice, length;

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        log_message("Socket creation failed");
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        log_message("Connection to server failed");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    log_message("Connected to server");

    while (1) {
        printf("\n----------------------------Menu:----------------------------\n");
        printf("1. Generate Username\n");
        printf("2. Generate Password\n");
        printf("3. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        if (choice == 3) {
            log_message("Exiting client");
            break;
        }
        if (choice == 1) {
            printf("Enter the desired length: ");
            scanf("%d", &length);
            sprintf(buffer, "username %d", length);
        } else if (choice == 2) {
            printf("Enter the desired length: ");
            scanf("%d", &length);
            sprintf(buffer, "password %d", length);
        } else {
            log_message("Invalid choice");
            printf("Error: Invalid choice. Please select 1, 2, or 3.\n");
            continue;
        }
        send_request(clientSocket, buffer);
        int recvLen = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (recvLen > 0) {
            buffer[recvLen] = '\0';
            log_message("Received response from server");
            printf("Response: %s\n", buffer);
        }
    }

    closesocket(clientSocket);
    WSACleanup();
    log_message("Client shutdown");
    return 0;
}
