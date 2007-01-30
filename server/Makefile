all: tcpmuxserver

clean:
	$(RM) *.o tcpmuxserver tcpmuxserver.exe

tcpmuxserver: ../common/queue.o ../common/tslog.o tcpmuxserver.o
	$(CC) $(CFLAGS) $+ -o $@ -lpthread
