CFLAGS = -wall

LOBJECTS = \
	structs.o			\
	thread_incoming_controller.o	\
	thread_incoming_reader.o	\
	thread_incoming_writer.o	\
	thread_outgoing_controller.o	\
	thread_outgoing_reader.o	\
	thread_outgoing_writer.o	\
	tcpmuxclient.o
GOBJECTS = \
	../common/queue.o		\
	../common/tslog.o		\
	$(LOBJECTS)

all: tcpmuxclient

clean:
	$(RM) $(LOBJECTS) tcpmuxclient tcpmuxclient.exe

tcpmuxclient: $(GOBJECTS)
	$(CC) $(CFLAGS) -lpthread $(GOBJECTS) -o $@
