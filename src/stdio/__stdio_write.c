#include "stdio_impl.h"
#include <sys/uio.h>

size_t write(int fd, const void *buf, size_t count);

ssize_t writev(int fd, const struct iovec *iov, int iovcnt){
  ssize_t cnt = 0, cnt2;
  if (iovcnt == 0)
    return 0;
  while (iovcnt--) {
    // Check if the buffer has space
    if (iov->iov_len == 0){
      iov++;
      continue;
    }

    cnt2 = write(fd, iov->iov_base, iov->iov_len);

    // Check for error and return
    if (cnt2 < 0)
      return cnt2;

    cnt += cnt2;
    // Check for less characters than sie of the buffer
    // Break in that case
    if (cnt < iov->iov_len)
      break;
    iov++;
  }

  return cnt;
}

size_t __stdio_write(FILE *f, const unsigned char *buf, size_t len)
{
  struct iovec iovs[2] = {
		{ .iov_base = f->wbase, .iov_len = f->wpos-f->wbase },
		{ .iov_base = (void *)buf, .iov_len = len }
	};
	struct iovec *iov = iovs;
	size_t rem = iov[0].iov_len + iov[1].iov_len;
	int iovcnt = 2;
	ssize_t cnt;
	for (;;) {
	  cnt = writev(f->fd, iov, iovcnt);
		if (cnt == rem) {
			f->wend = f->buf + f->buf_size;
			f->wpos = f->wbase = f->buf;
			return len;
		}
		if (cnt < 0) {
			f->wpos = f->wbase = f->wend = 0;
			f->flags |= F_ERR;
			return iovcnt == 2 ? 0 : len-iov[0].iov_len;
		}
		rem -= cnt;
		if (cnt > iov[0].iov_len) {
			cnt -= iov[0].iov_len;
			iov++; iovcnt--;
		}
		iov[0].iov_base = (char *)iov[0].iov_base + cnt;
		iov[0].iov_len -= cnt;
	}
}
