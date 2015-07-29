SET(SOURCE_FILE
    ../common/log.c
    ../common/log.h
    ../common/msg_proc_app.c
    ../common/msg_proc_app.h
    ../common/mqtt.c
    ../common/mqtt.h
    ../common/yunba_push.c
    ../common/yunba_push.h
    ../common/curl.c
    ../common/curl.h
    ../common/session.h
    ../common/session.c
    ../common/object.c
    ../common/object.h

    ../3rd/cJSON/cJSON.c
    ../3rd/cJSON/cJSON.h

    main.c
    server_tk115.c
    server_tk115.h
    msg_sch_tk115.c
    msg_sch_tk115.h
    msg_proc_tk115.c
    msg_proc_tk115.h
    msg_tk115.c
    msg_tk115.h)

MESSAGE(STATUS "configure Server for TK115")
add_executable(tk115_server ${SOURCE_FILE})
TARGET_LINK_LIBRARIES(tk115_server m event zlog curl mosquitto paho-mqtt3c)