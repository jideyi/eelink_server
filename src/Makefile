
CC= gcc -std=gnu99
CFLAGS= -O2 -Wall -Wextra 
LDFLAGS= 
LIBS= -levent


RM= rm -f


OBJS= main.o server_mc.o server_gizwits.o
TARGET= ../bin/server

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

clean:
	$(RM) $(TARGET) $(OBJS)



# list targets that do not create files (but not all makes understand .PHONY)
.PHONY: all clean

# objects
main.o: main.c server_gizwits.h server_mc.h
server_mc.o: server_mc.c server_mc.h
server_gizwits.o: server_gizwits.c server_gizwits.h
