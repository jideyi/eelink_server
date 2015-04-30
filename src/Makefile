
CC= gcc -std=gnu99
CFLAGS= -O2 -Wall -Wextra 
LDFLAGS= 
LIBS= -lm -levent -lzlog -lmosquitto -lcurl


RM= rm -f


OBJS= main.o server_mc.o log.o msg_sch_mc.o msg_proc_mc.o object_mc.o msg_mc.o http.o leancloud_req.o cJSON.o yeelink_req.o yeelink_rsp.o curl.o
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
log.o: log.c log.h
msg_sch_mc.o: msg_sch_mc.c msg_sch_mc.h msg_proc_mc.h msg_mc.h macro_mc.h cb_ctx_mc.h
msg_proc_mc.o: msg_proc_mc.c msg_proc_mc.h object_mc.h macro_mc.h cb_ctx_mc.h
object_mc.o: object_mc.c object_mc.h macro_mc.h
msg_mc.o: msg_mc.c msg_mc.h
http.o: http.c http.h
leancloud_req.o: leancloud_req.c leancloud_req.h
cJSON.o: cJSON.c cJSON.h
yeelink_req.o: yeelink_req.c yeelink_req.h
yeelink_rsp.o: yeelink_rsp.c yeelink_rsp.h
