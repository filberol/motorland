#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/motorsd_socket"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Create a Unix socket
    int sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockFd < 0) {
        perror("Cannot create socket");
        exit(EXIT_FAILURE);
    }

    // Set up socket address structure
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Connect to the server
    if (connect(sockFd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0) {
        perror("Error connecting to socket");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    // Send the message to the server
    ssize_t bytesSent = send(sockFd, argv[1], strlen(argv[1]), 0);
    if (bytesSent < 0) {
        perror("Error sending message");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    printf("Message sent successfully!\n");

    // Close the socket
    close(sockFd);

    return 0;
}