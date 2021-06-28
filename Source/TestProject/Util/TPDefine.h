#pragma once

typedef unsigned long ULONG;
typedef unsigned long DWORD;

#define PACKET_HEAD_SIZE 2
#define PACKET_END_SIZE 2
#define MAX_BUFF_SIZE 4096
#define BUFF_SIZE_IP 32
#define ERR_MSG_SIZE 256
#define SERVER_IP_SIZE 15

#define OP_ServerToClient 1
#define OP_ClientToServer 2

#define SIZE_USER_USER_ID 45
#define SIZE_USER_PASSWORD 45

#define MAX_RTT_MS 1000