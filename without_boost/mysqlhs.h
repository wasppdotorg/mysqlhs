
#include <stdio.h>
#include <string.h>

#ifndef _WIN32
#include <netdb.h> // addrinfo
#include <unistd.h> // close
#else
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif

#define MYSQL_HS_OK 0
#define MYSQL_HS_ERR -1
#define MYSQL_HS_ERR_WSA_START_UP_FAILED -2
#define MYSQL_HS_ERR_GET_ADDR_INFO_FAILED -3
#define MYSQL_HS_ERR_CONNECTION_FAILED -4
#define MYSQL_HS_ERR_QUERY_FAILED -5
#define MYSQL_HS_ERR_MALLOC_FAILED -6

//#define MYSQL_HS_BUF_LEN 8 // for test
#define MYSQL_HS_BUF_LEN 1024

typedef struct
{
	int result;
	int sockfd;

	int size;
	char* data;

} mysqlhs_context;

void _port2char(char* s, size_t n, const char* f, int p);

void _wsa_start_up(mysqlhs_context* c);
void _wsa_clean_up();
void _close(mysqlhs_context* c, int do_close, int do_wsa_clean_up);

mysqlhs_context* mysqlhs_connect(const char* host, int port);
void mysqlhs_close(mysqlhs_context* c);

void mysqlhs_execute(mysqlhs_context* c, const char* query);
