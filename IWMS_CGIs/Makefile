
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
LIBS     := -lsqlite3 -lm
Includes := -I ../IWMS_SQLs
Others   := cgic.c cJSON.c ../IWMS_SQLs/store.c ../IWMS_SQLs/common_time.c ./cgi_params.c ./cgi_response.c

all:$(OBJS)


$(OBJS):%.cgi:%.c
	$(CC) $(FLAGS) $< $(Others) -o $@ $(LIBS) $(Includes)
	cp $@ ~/IWMS/BOA/www/cgi-bin/

clean:
	rm $(OBJS)
