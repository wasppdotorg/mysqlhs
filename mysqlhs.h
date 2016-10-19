/*
Copyright (c) 2016-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef mysqlhs_h
#define mysqlhs_h

#ifndef _WIN32
#include <netdb.h> // addrinfo
#include <unistd.h> // close
#else
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif

#define MYSQL_HS_OK 0
#define MYSQL_HS_ERR -1
#define MYSQL_HS_ERR_INIT_FAILED -2
#define MYSQL_HS_ERR_GET_ADDR_INFO_FAILED -3
#define MYSQL_HS_ERR_CONNECTION_FAILED -4
#define MYSQL_HS_ERR_QUERY_FAILED -5
#define MYSQL_HS_ERR_MEMORY_ALLOC_FAILED -6

#define MYSQL_HS_BUF_LEN 7 - 1 // for test
//#define MYSQL_HS_BUF_LEN 1024 - 1

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	int result;
	int sockfd;

	int size;
	char* data;

} mysqlhs_context;

void port2char_(char* s, size_t n, const char* f, int i);

int init_();
void close_(mysqlhs_context* c, int do_close, int do_wsa_clean_up);

mysqlhs_context* mysqlhs_connect(const char* host, int port);
void mysqlhs_close(mysqlhs_context* c);

void mysqlhs_execute(mysqlhs_context* c, const char* query);

#ifdef __cplusplus
}
#endif

#endif // mysqlhs_h
