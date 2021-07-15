#include "ping.h"
#include <bits/types/struct_timespec.h>
#include <bits/types/struct_timeval.h>
#include <cstdint>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <thread>

#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SOCKET int
#define closesocket close
#define stricmp strcasecmp
#define strtok_s strtok_r

static uint32_t get_tick_count() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

// https://github.com/rockcarry/ping/blob/master/ping.c

#pragma pack(1)
typedef struct{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t seq;
    uint16_t data;
} ICMPPKT;
#pragma pack()

typedef struct {
    PFN_PING_CALLBACK   callback;
    void                *cbctxt;
    SOCKET              socket;
    struct sockaddr_in  dstaddr;
    #define FLAG_EXIT   (1 << 0)
    #define FLAG_START  (1 << 1)
    uint32_t            flags;
    uint32_t            ticksend;
    uint32_t            tickrecv;
    int32_t             interval;
    int32_t             timeout;
    std::thread         thread;
    ICMPPKT             icmppkt;
    uint8_t             recvbuf[256];
} PING;

static uint16_t checksum(uint8_t *buf, int len) {
    uint16_t *p16 = (uint16_t*) buf;
    uint32_t sum = 0;

    while (len >= 2) {
        sum += *p16++;
        len -= 2;
    }

    if (len) sum += *(uint8_t*)p16;

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return (uint16_t)~sum;
}

static void* ping_thread_proc(void *argv) {
    PING* ping = (PING*)argv;
    if (!argv) return nullptr;
    while (!(ping->flags & FLAG_EXIT)) {
        if (ping->flags & FLAG_START) {
            struct sockaddr_in srcaddr;
            uint32_t  tickcur = get_tick_count();
            socklen_t addrlen = sizeof(srcaddr);
            int32_t   bytes;
            if (ping->ticksend == 0 || (int32_t)tickcur - (int32_t)ping->ticksend >= ping->interval) {
                ping->icmppkt.seq++;
                ping->icmppkt.checksum = 0;
                ping->icmppkt.type     = 8;
                ping->icmppkt.data     = tickcur;
//              ping->icmppkt.id       = getpid();
                ping->icmppkt.checksum = checksum((uint8_t*)&ping->icmppkt, sizeof(ping->icmppkt));
                sendto(ping->socket, (char*)&ping->icmppkt, sizeof(ping->icmppkt), 0, (struct sockaddr*)&ping->dstaddr, sizeof(ping->dstaddr));
                ping->ticksend += ping->ticksend ? ping->interval : tickcur;
             printf("socket: %d, send icmp packet, seq: %d, checksum: %04X\n", ping->socket, ping->icmppkt.seq, ping->icmppkt.checksum); fflush(stdout);
            }
            bytes = recvfrom(ping->socket, (char*)ping->recvbuf, sizeof(ping->recvbuf), 0, (struct sockaddr*)&srcaddr, &addrlen);
            if (bytes > 0) {
                ICMPPKT   *pkt = (ICMPPKT*)(ping->recvbuf + 20);
                if (pkt->type == 0 && pkt->code == 0) {
                    ping->tickrecv = get_tick_count();
                    if (ping->callback) {
                        ping->callback(ping->cbctxt, inet_ntoa(srcaddr.sin_addr), bytes, ping->recvbuf[8], (int32_t)ping->tickrecv - (int32_t)pkt->data, pkt->seq);
                    }
                }
            }
        } else {
            ping->ticksend = 0;
            usleep(100 * 1000);
        }
    }
    return nullptr;
}

void* ping_init(int interval, int timeout, PFN_PING_CALLBACK callback, void *cbctx) {
    struct timeval tv = { 0, 100 * 1000 };

    PING *ping = (PING*)calloc(1, sizeof(PING));

    if (!ping) return nullptr;
    ping->interval = interval;
    ping->timeout = timeout;
    ping->callback = callback;
    ping->cbctxt = cbctx;
    ping->socket = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);

    // The setsockopt() function shall set the option specified by the option_name 
    // argument, at the protocol level specified by the level argument,
    // to the value pointed to by the option_value argument for 
    // the socket associated with the file descriptor specified 
    // by the socket argument.
    setsockopt(ping->socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));

    ping->thread = std::thread(ping_thread_proc, ping);
    return ping;
}

void ping_exit(void *ctx) {
    PING *ping = (PING*)ctx;
    if (!ctx) return;
    ping->flags |= FLAG_EXIT;
    ping->thread.join();
    if (ping->socket > 0) closesocket(ping->socket);
    free(ctx);
}

void ping_run(void *ctx, char *ip, int start) {
    PING *ping = (PING*)ctx;
    if (!ctx) return;
    if (ip) {
        ping->dstaddr.sin_family        = AF_INET;
        ping->dstaddr.sin_addr.s_addr   = inet_addr(ip);
    }

    if (start)  ping->flags |= FLAG_START;
    else        ping->flags &=~FLAG_START;
}

int ping_isok(void *ctx) {
    PING *ping = (PING*)ctx;
    return (ping && ping->tickrecv && (int32_t)get_tick_count() - (int32_t)ping->tickrecv < ping->timeout);
}

#ifdef _TEST_
static void ping_callback(void *cbctx, char *ip, int bytes, int ttl, int time, int seq) {
    printf("reply from %s, bytes: %d, ttl: %dms, time: %dms, seq: %d\n", ip, bytes, ttl, time, seq); fflush(stdout);
}

int main() {
    PING *ping = (PING*)ping_init(1000, 2000, ping_callback, nullptr);
    while (true) {
        char cmd[256], ip[256];
        scanf("%256s", cmd);
        if (strcmp(cmd, "ping") == 0) {
            scanf("%256s", ip);
            ping_run(ping, ip, 1);
        } else if (strcmp(cmd, "stop") == 0) {
            ping_run(ping, NULL, 0);
        } else if (strcmp(cmd, "isok") == 0) {
            printf("%d\n", ping_isok(ping)); fflush(stdout);
        } else if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
            break;
        }
    }
    ping_exit(ping);
    return 0;
}
#endif