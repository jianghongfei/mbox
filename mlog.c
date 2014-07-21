#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "mlog.h"

void
mlog_to_file(const char *time, const char *message)
{
	FILE *logfile;

	logfile = fopen(log_path, "a");
	if (!logfile) {
		printf("cannot open log");
		return;
	}

	fprintf(logfile, "[%s] %s\n", time, message);
	fclose(logfile);
}

void
mlog(const char *format, ...)
{
	char message[BUFFER_SIZE];
	char timestamp[BUFFER_SIZE];

	va_list args;

	time_t t;
	struct tm *tmptr;

	va_start(args, format);
	vsprintf(message, format, args);
	va_end(args);

	t = time(NULL);
	tmptr = localtime(&t);
	strftime(timestamp, BUFFER_SIZE, "%F %T", tmptr);

	mlog_to_file(timestamp, message);
}

