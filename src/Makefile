
CC= gcc -std=gnu99
CFLAGS= -O2 -Wall -Wextra 
LDFLAGS= 
LIBS= -lm -levent -lzlog -lmosquitto -lcurl


RM= rm -f


OBJS= main.o server_mc.o gizwits_req.o gizwits_rsp.o log.o msg_sch_mc.o msg_proc_mc.o object_mc.o msg_mc.o fsm.o http.o mqtt.o leancloud_req.o cJSON.o
TARGET= ../bin/server

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

clean:
	$(RM) $(TARGET) $(OBJS)



# list targets that do not create files (but not all makes understand .PHONY)
.PHONY: all clean

# objects
main.o: main.c server_mc.h
server_mc.o: server_mc.c server_mc.h
gizwits_req.o: gizwits_req.c gizwits_req.h
gizwits_rsp.o: gizwits_rsp.c gizwits_rsp.h
log.o: log.c log.h
msg_sch_mc.o: msg_sch_mc.c msg_sch_mc.h msg_proc_mc.h msg_mc.h macro_mc.h cb_ctx_mc.h
msg_proc_mc.o: msg_proc_mc.c msg_proc_mc.h object_mc.h macro_mc.h cb_ctx_mc.h
object_mc.o: object_mc.c object_mc.h macro_mc.h
msg_mc.o: msg_mc.c msg_mc.h
fsm.o: fsm.c fsm.h
http.o: http.c http.h
mqtt.o: mqtt.c mqtt.h
leancloud_req.o: leancloud_req.c leancloud_req.h
cJSON.o: cJSON.c cJSON.h
