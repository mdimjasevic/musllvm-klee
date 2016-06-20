#include <unistd.h>
#include <sys/ioctl.h>
#include "syscall.h"

int isatty(int fd)
{
	struct winsize wsz;
	return !ioctl(fd, TIOCGWINSZ, &wsz);
}
