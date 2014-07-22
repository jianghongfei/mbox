#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include "mlog.h"

#define RUNNING_DIR	"/tmp"
#define LOCK_FILE	"/var/lock/mbox.lock"

const char *log_path = "/tmp/mbox.log";

void log_message(char *filename, char *message)
{
	FILE *logfile;

	logfile = fopen(filename, "a");
	if (!logfile) {
		printf("cannot open log");
		return;
	}

	fprintf(logfile, "%s\n", message);
	fclose(logfile);
}

void signal_handler(int sig)
{
	switch (sig) {
	case SIGHUP:
		mlog("hangup signal catched");
		break;
	case SIGTERM:
		mlog("terminate signal catched");
		exit(EXIT_SUCCESS);
		break;
	}
}

void daemonize()
{
	int i, lfp;
	char str[10];

	if (getppid() == 1)
		return;		// already a daemon

	i = fork();
	if (i < 0)
		exit(EXIT_FAILURE);	// fork error
	if (i > 0)
		exit(EXIT_SUCCESS);	// parent exists

	// child (daemon) continues
	setsid();		// obtain a new process group

	for (i = getdtablesize(); i >= 0; --i)
		close(i);

	i = open("/dev/null", O_RDWR | O_CREAT, 0640);
	dup(i);
	dup(i);			// handle standard I/O
	umask(027);		// set newly created file permissions
	chdir(RUNNING_DIR);	// change running directory

	lfp = open(LOCK_FILE, O_RDWR | O_CREAT, 0640);
	if (lfp < 0)
		exit(EXIT_FAILURE);	// can not open
	if (lockf(lfp, F_TLOCK, 0) < 0) {
		mlog("Quit if already running");
		exit(EXIT_SUCCESS);	// can not lock
	}
	// first instance continues
	sprintf(str, "%d\n", getpid());
	write(lfp, str, strlen(str));	// record pid to lockfile

	signal(SIGCHLD, SIG_IGN);	// ignore child
	signal(SIGTSTP, SIG_IGN);	// ignore tty signals
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGHUP, signal_handler);	// catch hanup signal
	signal(SIGTERM, signal_handler);	// catch kill signal
}

main()
{
	daemonize();
	while (1) {
		sleep(1);	// run
		mlog("xxx");
	}
}

// EOF
