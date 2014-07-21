#ifndef __MLOG_H_
#define __MLOG_H_

#define BUFFER_SIZE	1024

extern const char *log_path;

void
mlog(const char *format, ...);

#endif
