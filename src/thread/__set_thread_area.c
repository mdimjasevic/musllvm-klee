#include "pthread_impl.h"

struct user_desc * lthread_info;
int __set_thread_area(void *p)
{
  lthread_info = p;
  return 0;
#ifdef SYS_set_thread_area
	return __syscall(SYS_set_thread_area, p);
#else
	return -ENOSYS;
#endif
}

struct pthread *__pthread_self()
{
  return lthread_info;
}
