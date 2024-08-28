#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 256

void generate_username(int length, char *result) {
    const char *vowels = "aeiou";
    const char *consonants = "bcdfghjklmnpqrstvwxyz";
    const int vowels_len = strlen(vowels);
    const int consonants_len = strlen(consonants);
    if (length < 5 || length > 15) {
        strcpy(result, "Error: Invalid username length");
        return;
    }
    srand(time(NULL));
    int start_with_vowel = rand() % 2;
    int is_vowel = start_with_vowel;
    for (int i = 0; i < length; ++i) {
        if (is_vowel) {
            result[i] = vowels[rand() % vowels_len];
        } else {
            result[i] = consonants[rand() % consonants_len];
        }
        is_vowel = !is_vowel;
    }
    result[length] = '\0';
}

void generate_password(int length, char *result) {
    const char *chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int chars_len = strlen(chars);
    if (length < 8 || length >= 50) {
        strcpy(result, "Error: Invalid password length");
        return;
    }
    srand(time(NULL));
    for (int i = 0; i < length; ++i) {
        result[i] = chars[rand() % chars_len];
    }
    result[length] = '\0';
}

void log_message(const char *message) {
    printf("[LOG]: %s\n", message);
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        log_message("Socket creation failed");
        return 1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        log_message("Bind failed");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        log_message("Listen failed");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    log_message("Server listening...");

    while ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen)) != INVALID_SOCKET) {
        log_message("Client connected");

        while (1) {
            int recvLen = recv(clientSocket, buffer, BUFFER_SIZE, 0);
            if (recvLen <= 0) {
                log_message("Client disconnected");
                break;
            }
            buffer[recvLen] = '\0';
            log_message("Received request from client");
            char command[BUFFER_SIZE];
            int length = 0;
            sscanf(buffer, "%s %d", command, &length);
            char result[BUFFER_SIZE];
            if (strcmp(command, "username") == 0) {
                generate_username(length, result);
                printf("Method: generate_username\n");
            } else if (strcmp(command, "password") == 0) {
                generate_password(length, result);
                printf("Method: generate_password\n");
            } else {
                strcpy(result, "Unknown request");
                log_message("Unknown request received");
            }
            printf("Response: %s\n", &result);
            send(clientSocket, result, strlen(result), 0);
            log_message("Response sent to client");
        }
        closesocket(clientSocket);
        log_message("Client connection closed");
    }
    closesocket(serverSocket);
    WSACleanup();
    log_message("Server shutdown");
    return 0;
}
