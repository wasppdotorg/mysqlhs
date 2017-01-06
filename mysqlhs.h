/*
Copyright (c) 2016-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef mysqlhs_h
#define mysqlhs_h

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <netdb.h> // addrinfo
#include <unistd.h> // close
#endif

#define MYSQL_HS_OK 0
#define MYSQL_HS_ERR -1
#define MYSQL_HS_ERR_QUERY_FAILED -2
#define MYSQL_HS_ERR_MEMORY_ALLOC_FAILED -3

#define MYSQL_HS_BUF_LEN 1024 - 1

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

mysqlhs_context* mysqlhs_connect(const char* host, int port);
void mysqlhs_execute(mysqlhs_context* c, const char* query);
void mysqlhs_close(mysqlhs_context* c);

int test_();

#ifdef __cplusplus
}
#endif

#endif // mysqlhs_h
