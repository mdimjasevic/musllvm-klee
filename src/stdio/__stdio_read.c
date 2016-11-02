#include "stdio_impl.h"
#include <sys/uio.h>

ssize_t read(int fd, void *buf, size_t count);
size_t __stdio_read(FILE *f, unsigned char *buf, size_t len)
{
	struct iovec iov[2] = {
		{ .iov_base = buf, .iov_len = len - !!f->buf_size },
		{ .iov_base = f->buf, .iov_len = f->buf_size }
	};
  ssize_t cnt, cntr2;
	// mimic readv implementation
  cnt = read(f->fd, iov[0].iov_base, iov[0].iov_len);
  if(cnt == iov[0].iov_len) {
    cntr2 = read(f->fd, iov[1].iov_base, iov[1].iov_len);
    if (cntr2 >= 0)
      cnt += cntr2;
    else
      cnt = cntr2;
  }

	if (cnt <= 0) {
		f->flags |= F_EOF ^ ((F_ERR^F_EOF) & cnt);
		return cnt;
	}
	if (cnt <= iov[0].iov_len) return cnt;
	cnt -= iov[0].iov_len;
	f->rpos = f->buf;
	f->rend = f->buf + cnt;
	if (f->buf_size) buf[len-1] = *f->rpos++;
	return len;
}
