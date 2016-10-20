/*
Copyright (c) 2016-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mysqlhs.h"

void port2char_(char* s, size_t n, const char* f, int i)
{
#ifndef _WIN32
	snprintf(s, n, f, i);
#else
	sprintf_s(s, n, f, i);
#endif
}

int init_()
{
#ifdef _WIN32
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != MYSQL_HS_OK)
	{
		return MYSQL_HS_ERR;
	}
#endif

	return MYSQL_HS_OK;
}

void close_(mysqlhs_context* c, int do_close, int do_wsa_clean_up)
{
#ifndef _WIN32
	if (do_close)
	{
		close(c->sockfd);
		c->sockfd = -1;
	}
#else
	if (do_close)
	{
		closesocket(c->sockfd);
		c->sockfd = -1;
	}

	if (do_wsa_clean_up)
	{
		WSACleanup();
	}
#endif
}

mysqlhs_context* mysqlhs_connect(const char* host, int port)
{
	mysqlhs_context* c;

	char port_[6]; // strlen("65535")
	struct addrinfo hints, *addr_info, *p;

	c = calloc(1, sizeof(mysqlhs_context));
	if (c == NULL)
	{
		return NULL;
	}

	c->result = MYSQL_HS_ERR;
	c->sockfd = -1;
	c->size = 0;
	c->data = (char*)malloc(sizeof(char) * MYSQL_HS_BUF_LEN);
	if (c->data == NULL)
	{
		c->result = MYSQL_HS_ERR_MEMORY_ALLOC_FAILED;
		return c;
	}

	if ((c->result = init_()) != MYSQL_HS_OK)
	{
		c->result = MYSQL_HS_ERR_INIT_FAILED;
		return c;
	}

	port2char_(port_, 6, "%d", port);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((c->result = getaddrinfo(host, port_, &hints, &addr_info)) != MYSQL_HS_OK)
	{
		close_(c, 0, 1);

		c->result = MYSQL_HS_ERR_GET_ADDR_INFO_FAILED;
		return c;
	}

	for (p = addr_info; p != NULL; p = p->ai_next)
	{
		if ((c->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == MYSQL_HS_ERR)
		{
			continue;
		}

		if (connect(c->sockfd, p->ai_addr, p->ai_addrlen) == MYSQL_HS_ERR)
		{
			close_(c, 1, 0);
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		close_(c, 0, 1);

		c->result = MYSQL_HS_ERR_CONNECTION_FAILED;
		return c;
	}

	freeaddrinfo(addr_info);

	c->result = MYSQL_HS_OK;
	return c;
}

void mysqlhs_close(mysqlhs_context* c)
{
	if (c == NULL)
	{
		return;
	}

	if (c->sockfd > 0)
	{
		close_(c, 1, 1);
	}

	if (c->data != NULL)
	{
		free(c->data);
	}

	free(c);
}

void mysqlhs_execute(mysqlhs_context* c, const char* query)
{
	int recv_size = 0;
	char buf[MYSQL_HS_BUF_LEN];
	char* reallocated;

	if ((c->result = send(c->sockfd, query, strlen(query), 0)) == MYSQL_HS_ERR)
	{
		close_(c, 1, 1);

		c->result = MYSQL_HS_ERR_QUERY_FAILED;
		return;
	}

	// clear
	memset(c->data, 0, c->size);
	c->size = 0;

	while ((recv_size = recv(c->sockfd, buf, MYSQL_HS_BUF_LEN, 0)) > 0)
	{
		if (c->size > 0)
		{
			// we use malloc, realloc is unstable sometimes.
			reallocated = (char*)malloc(sizeof(char) * (c->size + MYSQL_HS_BUF_LEN));
			if (reallocated == NULL)
			{
				c->result = MYSQL_HS_ERR_MEMORY_ALLOC_FAILED;
				return;
			}

			memcpy(reallocated, c->data, c->size);
			free(c->data);
			c->data = reallocated;
		}

		memcpy(c->data + c->size, buf, recv_size);
		c->size += recv_size;
		c->data[c->size] = '\0';

		if (recv_size < MYSQL_HS_BUF_LEN)
		{
			break;
		}
	}

	c->result = MYSQL_HS_OK;
}

int test_()
{
	mysqlhs_context* c = mysqlhs_connect("127.0.0.1", 9999);
	if (c->result)
	{
		printf("failed to connect");
		return 1;
	}

	mysqlhs_execute(c, "P	0	test	movie	PRIMARY	id,genre,title,view_count\n");
	if (c->result)
	{
		printf("failed to open");
		return 1;
	}
	printf("%s", c->data);

	mysqlhs_execute(c, "0	+	3	0	Sci-Fi	Star wars\n");
	if (c->result)
	{
		printf("failed to insert");
		return 1;
	}
	printf("%s", c->data);

	mysqlhs_execute(c, "0	>	1	0	10	0\n");
	if (c->result)
	{
		printf("failed to select");
		return 1;
	}
	printf("%s", c->data);
	
	mysqlhs_close(c);
	
	int z = getchar();
	return 0;
}