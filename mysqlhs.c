
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mysqlhs.h"

void _port2char(char* s, size_t n, const char* f, int p)
{
#ifndef _WIN32
	snprintf(s, n, f, p);
#else
	sprintf_s(s, n, f, p);
#endif
}

int _wsa_start_up()
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

void _wsa_clean_up()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

void _close(mysqlhs_context* c, int do_close, int do_wsa_clean_up)
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
		_wsa_clean_up();
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
	c->data = c->data = (char*)malloc((sizeof(char) * MYSQL_HS_BUF_LEN) + 1);
	if (c->data == NULL)
	{
		c->result = MYSQL_HS_ERR_MEMORY_ALLOC_FAILED;
		return c;
	}

	_port2char(port_, 6, "%d", port);
	if ((c->result = _wsa_start_up()) != MYSQL_HS_OK)
	{
		c->result = MYSQL_HS_ERR_WSA_START_UP_FAILED;
		return c;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((c->result = getaddrinfo(host, port_, &hints, &addr_info)) != MYSQL_HS_OK)
	{
		_close(c, 0, 1);

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
			_close(c, 1, 0);
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		_close(c, 0, 1);

		c->result = MYSQL_HS_ERR_CONNECTION_FAILED;
		return c;
	}

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
		_close(c, 1, 1);
	}

	if (c->data != NULL)
	{
		free(c->data);
	}

	free(c);
}

void mysqlhs_execute(mysqlhs_context* c, const char* query)
{
	int recv_cnt = 0;
	char buf[MYSQL_HS_BUF_LEN + 1];
	char* tmp;

	if ((c->result = send(c->sockfd, query, strlen(query), 0)) == MYSQL_HS_ERR)
	{
		_close(c, 1, 1);

		c->result = MYSQL_HS_ERR_QUERY_FAILED;
		return;
	}

	while (1)
	{
		if (c->size > 0)
		{
			// we used malloc, realloc is unstable sometimes.
			tmp = (char*)malloc((sizeof(char) * (c->size + MYSQL_HS_BUF_LEN)) + 1);
			if (tmp == NULL)
			{
				c->result = MYSQL_HS_ERR_MEMORY_ALLOC_FAILED;
				return;
			}
			memcpy(tmp, c->data, c->size);
			free(c->data);
			c->data = tmp;
		}

		recv_cnt = recv(c->sockfd, buf, MYSQL_HS_BUF_LEN, 0);
		c->size += recv_cnt;
		if (recv_cnt <= 0)
		{
			break;
		}

		memcpy(&(c->data[c->size - recv_cnt]), buf, recv_cnt);
		c->data[c->size] = '\0';
		
		if (recv_cnt < MYSQL_HS_BUF_LEN)
		{
			break;
		}
	}

	c->result = MYSQL_HS_OK;
}
