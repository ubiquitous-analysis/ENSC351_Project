#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// port the UDP lisens on & MUST match server.js
#define SCOREBOARD_PORT 12345

static int sock = -1;
static struct sockaddr_in dest;

void scoreboard_init(void)
{
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(SCOREBOARD_PORT);
    inet_pton(AF_INET, "127.0.0.1", &dest.sin_addr);
}

void scoreboard_send(int red, int blue)
{
    char msg[32];
    int len = snprintf(msg, sizeof(msg), "%d,%d", red, blue);
    sendto(sock, msg, len, 0, (struct sockaddr *)&dest, sizeof(dest));
}

void scoreboard_cleanup(void)
{
    close(sock);
}