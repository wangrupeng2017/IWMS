
#CC    := gcc
CC    := arm-none-linux-gnueabi-gcc
FLAGS := -Wall -g
OBJS  := main

#LIBS     := -lsqlite3 -lpthread
LIBS     := -L/home/linux/nfs/rootfs/lib -L/home/linux/nfs/rootfs/usr/lib -lsqlite3 -lpthread
Includes := -I ../IWMS_SQLs
Others   := ../IWMS_SQLs/store.c ../IWMS_SQLs/common_time.c ./value_dispose.c ./zigbee_uart.c

all:$(OBJS)


$(OBJS):%:%.c
	$(CC) $(FLAGS) $< $(Others) -o $@ $(LIBS) $(Includes)
	# cp $@ ~/www/
	cp $@ ~/nfs/rootfs/www/
	

clean:
	rm $(OBJS)
