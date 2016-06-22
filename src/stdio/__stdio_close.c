#include "stdio_impl.h"

static int dummy(int fd)
{
	return fd;
}

weak_alias(dummy, __aio_close);

int close(int fd) ;
int __stdio_close(FILE *f)
{
	return close(__aio_close(f->fd));
}
