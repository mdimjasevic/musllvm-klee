#include <unistd.h>
#include "syscall.h"
#include "libc.h"

off_t __fd_lseek(int fd, off_t offset, int whence);
off_t lseek(int fd, off_t offset, int whence)
{
#ifdef SYS__llseek
	off_t result;
	return syscall(SYS__llseek, fd, offset>>32, offset, &result, whence) ? -1 : result;
#else
	return __fd_lseek(fd, offset, whence);
#endif
}

LFS64(lseek);
