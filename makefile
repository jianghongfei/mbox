mbox: mbox.o mlog.o
	cc -o mbox mbox.o mlog.o

mbox.o : mbox.c mlog.h
	cc -c mbox.c

mlog.o : mlog.c mlog.h
	cc -c mlog.c

clean :
	rm mbox mbox.o mlog.o
