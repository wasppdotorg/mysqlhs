
#include <stdio.h>

#include "mysqlhs.h"

int main()
{
	mysqlhs_context* c = mysqlhs_connect("www.geeksen.com", 80);

	const char* query = "GET / HTTP/1.1\nHost:localhost\nContent-Length: 0\n\n";
	mysqlhs_execute(c, query);

	printf("%s\n", c->data);

	mysqlhs_close(c);

	int c_ = getchar();
	return 0;
}
