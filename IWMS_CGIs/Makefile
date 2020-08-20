
CC    := gcc
FLAGS := -Wall -g
OBJS  := test.cgi 
OBJS  += realtime_temperature_param.cgi realtime_humidity_param.cgi realtime_illumination_param.cgi
OBJS  += realtime_device_status.cgi 
OBJS  += update_temperature_config.cgi update_humidity_config.cgi update_illumination_config.cgi
OBJS  += realtime_trend_data.cgi realtime_statistics_data.cgi
OBJS  += history_trend_data.cgi history_statistics_data.cgi
OBJS  += update_device_config.cgi
#LIBS=-L ./lib -lsqlite3
LIBS=-lsqlite3 -lm

all:$(OBJS)


$(OBJS):%.cgi:%.c
	$(CC) $(FLAGS) $< cgic.c cJSON.c -o $@ $(LIBS)
	cp $@ ~/IWMS/BOA/www/cgi-bin/

clean:
	rm $(OBJS)
