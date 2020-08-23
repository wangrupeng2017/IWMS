
# CC    := gcc
CC    := arm-none-linux-gnueabi-gcc
FLAGS := -Wall -g
OBJS  += realtime_temperature_param.cgi realtime_humidity_param.cgi realtime_illumination_param.cgi
OBJS  += realtime_device_status.cgi 
OBJS  += update_temperature_config.cgi update_humidity_config.cgi update_illumination_config.cgi
OBJS  += realtime_trend_data.cgi realtime_statistics_data.cgi
OBJS  += history_trend_data.cgi history_statistics_data.cgi
OBJS  += update_device_config.cgi


# LIBS     := -lsqlite3 -lm
LIBS     := -L/home/linux/nfs/rootfs/lib -L/home/linux/nfs/rootfs/usr/lib -lsqlite3 -lm
Includes := -I ../IWMS_SQLs
Others   := cgic.c cJSON.c ../IWMS_SQLs/store.c ../IWMS_SQLs/common_time.c ./cgi_params.c ./cgi_response.c

all:$(OBJS)


$(OBJS):%.cgi:%.c
	$(CC) $(FLAGS) $< $(Others) -o $@ $(LIBS) $(Includes)
	# cp $@ ~/www/cgi-bin/
	cp $@ ~/nfs/rootfs/www/cgi-bin/

clean:
	rm $(OBJS)
